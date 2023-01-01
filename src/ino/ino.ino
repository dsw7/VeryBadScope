#include "command_connection_test.h"
#include "core_primitives.h"

const unsigned int BAUD_RATE = 19200;
const unsigned int MAX_TIME_MILLISEC_WAIT_SERIAL_DATA = 10;

namespace Core
{

void read_analog_pin(const String &command)
{
    // Command should be of form: read:<num-reads>:<range-in-usecs>

    if (command.length() < 6)
    {
        Primitives::error("Malformed read command!");
        return;
    }

    int idx_col_1 = command.indexOf(':');
    int idx_col_2 = command.indexOf(':', idx_col_1 + 1);

    long n_reads = command.substring(idx_col_1 + 1, idx_col_2).toInt();

    if (n_reads == 0)
    {
        Primitives::error("Could not parse number of reads!");
        return;
    }
    else if (n_reads < 5)
    {
        Primitives::error("Minimum of 5 reads required!");
        return;
    }

    long range = command.substring(idx_col_2 + 1).toInt();

    if (range == 0)
    {
        Primitives::error("Could not parse range!");
        return;
    }
    else if (range < 1000)
    {
        Primitives::error("Minimum range is 1000 microseconds!");
        return;
    }

    static unsigned int read_pin = A0;

    unsigned int read_results[n_reads] = {0};
    unsigned long read_times_usec[n_reads] = {0};

    // On UNO, it takes about one hundred microseconds to read analog input
    // See: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    // From my experimentation, the read time delay is 112 usecs
    long period = (range / n_reads) - 112;

    // See: https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
    // For a description of these magic numbers
    if (period < 3)
    {
        Primitives::error("Computed period is too short. Try a greater range to count ratio!");
        return;
    }
    else if (period > 16383)
    {
        Primitives::error("Computed period is too long. Try a lesser range to count ratio!");
        return;
    }

    for (unsigned int i = 0; i < n_reads; ++i)
    {
        read_results[i] = ::analogRead(A0);
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

} // namespace Core

void setup()
{
    ::Serial.begin(::BAUD_RATE);
    ::Serial.setTimeout(::MAX_TIME_MILLISEC_WAIT_SERIAL_DATA);
    ::pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    while (::Serial.available() > 0)
    {
        ::String command = ::Serial.readString();
        command.trim();

        if (command == "hello")
        {
            Command::run_connection_test();
        }
        else if (command.startsWith("read:"))
        {
            Core::read_analog_pin(command);
        }
        else if (command == "exit")
        {
            Primitives::info("Closing connection. Goodbye!");
            ::Serial.end();
            break;
        }
        else
        {
            Primitives::error("Unknown command: " + command);
        }
    }
}
