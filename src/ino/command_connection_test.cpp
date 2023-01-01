#include "command_connection_test.h"

#include "core_primitives.h"

#include "Arduino.h"

namespace Command
{

void run_connection_test()
{
    Primitives::info("Hello from InoDAQ2. I should blink 5 times!");

    for (unsigned int i = 0; i < 5; ++i)
    {
        ::digitalWrite(LED_BUILTIN, HIGH);
        ::delay(100);
        ::digitalWrite(LED_BUILTIN, LOW);
        ::delay(100);
    }
}

} // Command
