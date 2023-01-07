#include "command_edge.h"
#include "command_hello.h"
#include "command_roll.h"
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
            Command::Roll cmd;
            cmd.set_command(command);
            cmd.acquire_data();
        }
        else if (command.startsWith(F("edge:")))
        {
            Command::Edge cmd;
            cmd.set_command(command);
            cmd.acquire_data();
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
