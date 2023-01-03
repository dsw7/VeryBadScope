#include "command_connection_test.h"
#include "command_roll.h"
#include "command_trigger.h"
#include "helpers.h"

void setup()
{
    unsigned int baud_rate = 19200;
    unsigned int max_time_millisec_wait_serial_data = 10;

    ::Serial.begin(baud_rate);
    ::Serial.setTimeout(max_time_millisec_wait_serial_data);
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
        else if (command.startsWith(F("roll:")))
        {
            Command::Roll roll;
            roll.set_command(command);
            roll.acquire_data();
        }
        else if (command.startsWith(F("trigger:")))
        {
            Command::Trigger trigger;
            trigger.set_command(command);
            trigger.acquire_data();
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
