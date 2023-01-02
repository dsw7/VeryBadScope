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

void roll(const String &command)
{
    // Command should be of form: read:<num-reads>:<range-in-usecs>

    // 1. validate read length
    if (command.length() < 6)
    {
        Helpers::error("Malformed read command!");
        return;
    }

    // 2. get indices
    int idx_col_1 = command.indexOf(':');
    int idx_col_2 = command.indexOf(':', idx_col_1 + 1);

    // 3. get number of reads
    long n_reads = command.substring(idx_col_1 + 1, idx_col_2).toInt();

    if (n_reads == 0)
    {
        Helpers::error("Could not parse number of reads!");
        return;
    }
    else if (n_reads < 5)
    {
        Helpers::error("Minimum of 5 reads required!");
        return;
    }

    // 4. get timebase
    long range = command.substring(idx_col_2 + 1).toInt();

    if (range == 0)
    {
        Helpers::error("Could not parse range!");
        return;
    }
    else if (range < 1000)
    {
        Helpers::error("Minimum range is 1000 microseconds!");
        return;
    }

    // 5. get period

    // On UNO, it takes about one hundred microseconds to read analog input
    // See: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    // From my experimentation, the read time delay is 112 usecs
    long period = (range / n_reads) - 112;

    // See: https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
    // For a description of these magic numbers
    if (period < 3)
    {
        Helpers::error("Computed period is too short. Try a greater range to count ratio!");
        return;
    }
    else if (period > 16383)
    {
        Helpers::error("Computed period is too long. Try a lesser range to count ratio!");
        return;
    }

    unsigned int read_results[n_reads] = {0};
    unsigned long read_times_usec[n_reads] = {0};
    static unsigned int read_pin = A0;

    for (unsigned int i = 0; i < n_reads; ++i)
    {
        read_results[i] = ::analogRead(read_pin);
        read_times_usec[i] = ::micros();
        ::delayMicroseconds(period);
    }

    ::Serial.print("1;");
    for (unsigned int i = 0; i < n_reads; ++i)
    {
        ::Serial.print(read_results[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();

    ::Serial.print("1;");
    for (unsigned int i = 0; i < n_reads; ++i)
    {
        ::Serial.print(read_times_usec[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();
}

} // namespace Command
