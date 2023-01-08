#include "command_level.h"

#include "helpers.h"

namespace Command
{

bool Level::parse_level_command_specific_indices()
{
    // Ensure that "rising" can be parsed from "level:5:1000:rising:818:15"
    this->idx_trigger = this->command.indexOf(':', this->idx_measurement_duration + 1);

    if (this->idx_trigger < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    // Ensure that "818" can be parsed from "level:5:1000:rising:818:15"
    this->idx_trigger_level = this->command.indexOf(':', this->idx_trigger + 1);

    if (this->idx_trigger_level < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    // Ensure that "15" can be parsed from "level:5:1000:rising:818:15"
    this->idx_epsilon = this->command.indexOf(':', this->idx_trigger_level + 1);

    if (this->idx_epsilon < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    return true;
}

bool Level::parse_trigger()
{
    // Parse "rising" from incoming command "level:5:1000:rising:818:15"

    this->trigger_type = command.substring(this->idx_trigger + 1, this->idx_trigger_level);

    if (this->trigger_type == 0)
    {
        Helpers::error(F("Could not parse trigger type!"));
        return false;
    }

    if (this->trigger_type.equals("rising") or this->trigger_type.equals("falling"))
    {
        return true;
    }

    Helpers::error(F("Invalid trigger type!"));
    return false;
}

bool Level::parse_trigger_level()
{
    // Parse "818" from incoming command "level:5:1000:rising:818:15"

    this->trigger_level = command.substring(this->idx_trigger_level + 1).toInt();

    if (this->trigger_level == 0)
    {
        Helpers::error(F("Could not parse trigger level!"));
        return false;
    }

    if (this->trigger_level < 5)
    {
        Helpers::error(F("Trigger level must be at least 0.025 volts!"));
        return false;
    }
    else if (this->trigger_level > 1023)
    {
        Helpers::error(F("Trigger level cannot exceed 5 volts!"));
        return false;
    }

    return true;
}

bool Level::parse_epsilon()
{
    // Parse "15" from incoming command "level:5:1000:rising:818:15"

    this->epsilon = command.substring(this->idx_epsilon + 1).toInt();

    if (this->epsilon == 0)
    {
        Helpers::error(F("Could not parse epsilon!"));
        return false;
    }

    if (this->epsilon < 5)
    {
        Helpers::error(F("Epsilon must be at least 0.025 volts!"));
        return false;
    }
    else if (this->epsilon > 1023)
    {
        Helpers::error(F("Epsilon cannot exceed 5 volts!"));
        return false;
    }

    return true;
}

void Level::trigger()
{
    int v_t[this->record_length] = {0};
    unsigned long time_usec[this->record_length] = {0};

    int v_t_a = 0;
    int v_t_b = ::analogRead(this->read_pin);

    // https://www.arduino.cc/reference/en/language/functions/math/abs/
    // See notes and warnings - not a good idea to compute delta inside abs
    int delta = 0;

    unsigned long t = 0;

    bool count = false;
    unsigned int idx = 0;

    if (this->trigger_type.equals("rising"))
    {
        while (idx < this->record_length)
        {
            v_t_a = v_t_b;
            v_t_b = ::analogRead(this->read_pin);

            t = ::micros();
            ::delayMicroseconds(this->period);

            delta = v_t_b - this->trigger_level;

            if ((v_t_a < v_t_b) and (abs(delta) <= this->epsilon))
            {
                count = true;
            }

            if (count)
            {
                time_usec[idx] = t;
                v_t[idx] = v_t_b;
                ++idx;
            }
        }
    }
    else
    {
        while (idx < this->record_length)
        {
            v_t_a = v_t_b;
            v_t_b = ::analogRead(this->read_pin);

            t = ::micros();
            ::delayMicroseconds(this->period);

            delta = v_t_b - this->trigger_level;

            if ((v_t_a > v_t_b) and (abs(delta) <= this->epsilon))
            {
                count = true;
            }

            if (count)
            {
                time_usec[idx] = t;
                v_t[idx] = v_t_b;
                ++idx;
            }
        }
    }

    ::Serial.print(F("1;"));
    for (unsigned int i = 0; i < this->record_length; ++i)
    {
        ::Serial.print(v_t[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();

    ::Serial.print(F("1;"));
    for (unsigned int i = 0; i < this->record_length; ++i)
    {
        ::Serial.print(time_usec[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();
}

void Level::acquire_data()
{
    if (not this->parse_command_indices())
    {
        return;
    }

    if (not this->parse_level_command_specific_indices())
    {
        return;
    }

    if (not this->parse_record_length())
    {
        return;
    }

    if (not this->parse_measurement_duration())
    {
        return;
    }

    if (not this->parse_trigger())
    {
        return;
    }

    if (not this->parse_trigger_level())
    {
        return;
    }

    if (not this->parse_epsilon())
    {
        return;
    }

    if (not this->compute_period())
    {
        return;
    }

    this->trigger();
}

} // namespace Command
