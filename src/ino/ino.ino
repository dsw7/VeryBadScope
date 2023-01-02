#include "command_connection_test.h"
#include "command_roll.h"
#include "helpers.h"

const unsigned int BAUD_RATE = 19200;
const unsigned int MAX_TIME_MILLISEC_WAIT_SERIAL_DATA = 10;

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

        if (command == F("hello"))
        {
            Command::run_connection_test();
        }
        else if (command.startsWith(F("read:")))
        {
            Command::Roll roll;
            roll.set_command(command);
            roll.acquire_data();
        }
        else if (command == F("exit"))
        {
            Helpers::info(F("Closing connection. Goodbye!"));
            ::Serial.end();
            break;
        }
        else
        {
            Helpers::error("Unknown command: " + command);
        }
    }
}
