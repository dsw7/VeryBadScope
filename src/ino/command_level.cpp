#include "command_level.h"

#include "helpers.h"

namespace Command
{

bool Level::parse_level_command_specific_indices()
{
    // Ensure that "rising" can be parsed from "level:5:1000:rising:818"
    this->idx_trigger = this->command.indexOf(':', this->idx_measurement_duration + 1);

    if (this->idx_trigger < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    // Ensure that "818" can be parsed from "level:5:1000:rising:818"
    this->idx_trigger_level = this->command.indexOf(':', this->idx_trigger + 1);

    if (this->idx_trigger_level < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    return true;
}

bool Level::parse_trigger()
{
    // Parse "rising" from incoming command "level:5:1000:rising:818"

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
    // Parse "818" from incoming command "level:5:1000:rising:818"

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

void Level::trigger()
{
    static unsigned int read_pin = A0;

    int v_t[this->record_length] = {0};
    unsigned long time_usec[this->record_length] = {0};

    int v_t_a = 0;
    int v_t_b = ::analogRead(read_pin);
    unsigned long t = 0;

    bool count = false;
    unsigned int idx = 0;

    if (this->trigger_type.equals("rising"))
    {
        while (idx < this->record_length)
        {
            v_t_a = v_t_b;
            v_t_b = ::analogRead(read_pin);

            t = ::micros();
            ::delayMicroseconds(this->period);

            if ((v_t_a < v_t_b) and (v_t_b - this->trigger_level < 0.05))
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
            v_t_b = ::analogRead(read_pin);

            t = ::micros();
            ::delayMicroseconds(this->period);

            if ((v_t_a > v_t_b) and (v_t_b - this->trigger_level < 0.05))
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

    if (not this->compute_period())
    {
        return;
    }

    this->trigger();
}

} // namespace Command
