#include "command_scope_base.h"

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
        Helpers::error(F("Malformed read command!"));
        return false;
    }

    return true;
}

bool ScopeBase::parse_command_indices()
{
    this->idx_record_length = this->command.indexOf(':');

    if (this->idx_record_length < 0)
    {
        Helpers::error(F("Malformed read command! Missing one or more colons"));
        return false;
    }

    this->idx_measurement_duration = this->command.indexOf(':', this->idx_record_length + 1);

    if (this->idx_measurement_duration < 0)
    {
        Helpers::error(F("Malformed read command! Missing one or more colons"));
        return false;
    }

    return true;
}

bool ScopeBase::parse_record_length()
{
    this->record_length = command.substring(this->idx_record_length + 1, this->idx_measurement_duration).toInt();

    if (this->record_length == 0)
    {
        Helpers::error(F("Could not parse record length!"));
        return false;
    }
    else if (this->record_length < 5)
    {
        Helpers::error(F("Record length must be at least 5 reads!"));
        return false;
    }

    return true;
}

bool ScopeBase::parse_measurement_duration()
{
    this->measurement_duration = command.substring(this->idx_measurement_duration + 1).toInt();

    if (this->measurement_duration == 0)
    {
        Helpers::error(F("Could not parse measurement duration!"));
        return false;
    }
    else if (this->measurement_duration < 1000)
    {
        Helpers::error(F("Minimum measurement duration is 1000 microseconds!"));
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
        Helpers::error(F("Computed period is too short. Try a greater range to count ratio!"));
        return false;
    }
    else if (this->period > 16383)
    {
        Helpers::error(F("Computed period is too long. Try a lesser range to count ratio!"));
        return false;
    }

    return true;
}

} // namespace Command
