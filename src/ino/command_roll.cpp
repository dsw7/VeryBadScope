#include "command_roll.h"

#include "helpers.h"

namespace Command
{

void ScopeBase::set_command(const ::String &command)
{
    this->command = command;
}

bool ScopeBase::validate_command()
{
    if (this->command.length() < 6)
    {
        Helpers::error("Malformed read command!");
        return false;
    }

    return true;
}

bool ScopeBase::parse_command_indices()
{
    this->idx_record_length = this->command.indexOf(':');

    if (this->idx_record_length < 0)
    {
        Helpers::error("Malformed read command! Missing one or more colons");
        return false;
    }

    this->idx_measurement_duration = this->command.indexOf(':', this->idx_record_length + 1);

    if (this->idx_measurement_duration < 0)
    {
        Helpers::error("Malformed read command! Missing one or more colons");
        return false;
    }

    return true;
}

bool ScopeBase::parse_record_length()
{
    this->record_length = command.substring(this->idx_record_length + 1, this->idx_measurement_duration).toInt();

    if (this->record_length == 0)
    {
        Helpers::error("Could not parse record length!");
        return false;
    }
    else if (this->record_length < 5)
    {
        Helpers::error("Record length must be at least 5 reads!");
        return false;
    }

    return true;
}

bool ScopeBase::parse_measurement_duration()
{
    this->measurement_duration = command.substring(this->idx_measurement_duration + 1).toInt();

    if (this->measurement_duration == 0)
    {
        Helpers::error("Could not parse measurement duration!");
        return false;
    }
    else if (this->measurement_duration < 1000)
    {
        Helpers::error("Minimum measurement duration is 1000 microseconds!");
        return false;
    }

    return true;
}

bool ScopeBase::compute_period()
{
    // On UNO, it takes about one hundred microseconds to read analog input
    // See: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    // From my experimentation, the read time delay is 112 usecs
    this->period = (this->measurement_duration / this->record_length) - 112;

    // See: https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
    // For a description of these magic numbers
    if (this->period < 3)
    {
        Helpers::error("Computed period is too short. Try a greater range to count ratio!");
        return false;
    }
    else if (this->period > 16383)
    {
        Helpers::error("Computed period is too long. Try a lesser range to count ratio!");
        return false;
    }

    return true;
}

void Roll::roll()
{
    static unsigned int read_pin = A0;

    unsigned int v_t[this->record_length] = {0};
    unsigned long time_usec[this->record_length] = {0};

    for (unsigned int i = 0; i < this->record_length; ++i)
    {
        v_t[i] = ::analogRead(read_pin);
        time_usec[i] = ::micros();
        ::delayMicroseconds(this->period);
    }

    ::Serial.print("1;");
    for (unsigned int i = 0; i < this->record_length; ++i)
    {
        ::Serial.print(v_t[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();

    ::Serial.print("1;");
    for (unsigned int i = 0; i < this->record_length; ++i)
    {
        ::Serial.print(time_usec[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();
}

void Roll::acquire_data()
{
    if (not this->validate_command())
    {
        return;
    }

    if (not this->parse_command_indices())
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

    if (not this->compute_period())
    {
        return;
    }

    this->roll();
}

} // namespace Command
