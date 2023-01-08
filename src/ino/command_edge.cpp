#include "command_edge.h"

#include "helpers.h"

namespace Command
{

bool Edge::parse_edge_command_specific_indices()
{
    // Ensure that "rising" can be parsed from "edge:5:1000:rising:818"
    this->idx_trigger = this->command.indexOf(':', this->idx_measurement_duration + 1);

    if (this->idx_trigger < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    // Ensure that "818" can be parsed from "edge:5:1000:rising:818"
    this->idx_trigger_delta = this->command.indexOf(':', this->idx_trigger + 1);

    if (this->idx_trigger_delta < 0)
    {
        Helpers::error(F("Malformed command! Missing one or more colons"));
        return false;
    }

    return true;
}

bool Edge::parse_trigger()
{
    // Parse "rising" from incoming command "edge:5:1000:rising:818"

    this->trigger_type = command.substring(this->idx_trigger + 1, this->idx_trigger_delta);

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

bool Edge::parse_trigger_delta()
{
    // Parse "818" from incoming command "edge:5:1000:rising:818"

    this->trigger_delta = command.substring(this->idx_trigger_delta + 1).toInt();

    if (this->trigger_delta == 0)
    {
        Helpers::error(F("Could not parse trigger delta!"));
        return false;
    }

    if (this->trigger_delta < 20)
    {
        Helpers::error(F("Trigger delta must be at least 0.1 volts!"));
        return false;
    }
    else if (this->trigger_delta > 1023)
    {
        Helpers::error(F("Trigger delta cannot exceed 5 volts!"));
        return false;
    }

    return true;
}

void Edge::wait_for_trigger()
{
    int v_t_a = 0;
    int v_t_b = ::analogRead(this->read_pin);

    long uncorrected_period = this->measurement_duration / this->record_length;

    if (this->trigger_type.equals("rising"))
    {
        while (true)
        {
            v_t_a = v_t_b;
            v_t_b = ::analogRead(this->read_pin);

            ::delayMicroseconds(uncorrected_period);

            if ((v_t_b - v_t_a) >= this->trigger_delta)
            {
                break;
            }
        }
    }
    else
    {
        while (true)
        {
            v_t_a = v_t_b;
            v_t_b = ::analogRead(this->read_pin);

            ::delayMicroseconds(uncorrected_period);

            if ((v_t_a - v_t_b) >= this->trigger_delta)
            {
                break;
            }
        }
    }
}

void Edge::start_reading_after_trigger()
{
    int v_t[this->record_length] = {0};
    unsigned long time_usec[this->record_length] = {0};

    unsigned int idx = 0;

    while (idx < this->record_length)
    {
        time_usec[idx] = ::micros();
        v_t[idx] = ::analogRead(this->read_pin);
        ++idx;

        ::delayMicroseconds(this->corrected_period);
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

void Edge::acquire_data()
{
    if (not this->parse_command_indices())
    {
        return;
    }

    if (not this->parse_edge_command_specific_indices())
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

    if (not this->parse_trigger_delta())
    {
        return;
    }

    if (not this->compute_period())
    {
        return;
    }

    this->wait_for_trigger();
    this->start_reading_after_trigger();
}

} // namespace Command
