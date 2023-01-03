#include "command_trigger.h"

namespace Command
{

void Trigger::trigger()
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

void Trigger::acquire_data()
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

    this->trigger();
}

} // namespace Command
