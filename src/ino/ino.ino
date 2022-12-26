const unsigned int BAUD_RATE = 19200;
const unsigned int MAX_TIME_MILLISEC_WAIT_SERIAL_DATA = 10;

namespace Core
{

void run_connection_test()
{
    ::Serial.println("Hello from InoDAQ2. I should blink 5 times!");
    ::Serial.flush();

    for (unsigned int i = 0; i < 5; ++i)
    {
        ::digitalWrite(LED_BUILTIN, HIGH);
        ::delay(100);
        ::digitalWrite(LED_BUILTIN, LOW);
        ::delay(100);
    }
}

void read_analog_pin(const String &command)
{
    if (command.length() < 6)
    {
        ::Serial.println("Invalid input!");
        ::Serial.flush();
        return;
    }

    long num_reads = command.substring(command.indexOf(':') + 1).toInt();

    if (num_reads == 0)
    {
        ::Serial.println("Could not parse number of reads!");
        ::Serial.flush();
        return;
    }
    else if (num_reads < 0)
    {
        ::Serial.println("Parsed a negative number of reads!");
        ::Serial.flush();
        return;
    }

    static unsigned int read_pin = A0;
    unsigned int read_results[num_reads];

    // On UNO, it takes about one hundred microseconds to read analog input
    // See: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    static unsigned int delay_read = 100;

    for (unsigned int i = 0; i < num_reads; ++i)
    {
        read_results[i] = ::analogRead(A0);
        ::delayMicroseconds(delay_read);
    }

    for (unsigned int i = 0; i < num_reads; ++i)
    {
        ::Serial.print(read_results[i]);
        ::Serial.print(' ');
    }

    ::Serial.println();
    ::Serial.flush();
}

void exit_program()
{
    ::Serial.println("Closing connection. Goodbye!");
    ::Serial.flush();
    ::Serial.end();
}

void handle_unknown_command(const String &command)
{
    ::Serial.println("Unknown command: " + command);
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
            Core::run_connection_test();
        }
        else if (command.startsWith("read:"))
        {
            Core::read_analog_pin(command);
        }
        else if (command == "exit")
        {
            Core::exit_program();
            break;
        }
        else
        {
            Core::handle_unknown_command(command);
        }
    }
}
