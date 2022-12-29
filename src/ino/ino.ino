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
    // Command should be of form: read:<num-reads>:<time-range>

    if (command.length() < 6)
    {
        ::Serial.println("Invalid input!");
        ::Serial.flush();
        return;
    }

    int idx_col_1 = command.indexOf(':');
    int idx_col_2 = command.indexOf(':', idx_col_1 + 1);

    long n_reads = command.substring(idx_col_1 + 1, idx_col_2).toInt();

    if (n_reads == 0)
    {
        ::Serial.println("Could not parse number of reads!");
        ::Serial.flush();
        return;
    }
    else if (n_reads < 0)
    {
        ::Serial.println("Parsed a negative number of reads!");
        ::Serial.flush();
        return;
    }

    static unsigned int read_pin = A0;

    unsigned int read_results[n_reads];
    unsigned long read_times_usec[n_reads];

    // On UNO, it takes about one hundred microseconds to read analog input
    // See: https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/
    static unsigned int delay_read = 100;

    unsigned long start_time = ::micros();

    for (unsigned int i = 0; i < n_reads; ++i)
    {
        read_results[i] = ::analogRead(A0);
        read_times_usec[i] = ::micros(); // note that micros is slow
        ::delayMicroseconds(delay_read);
    }

    for (unsigned int i = 0; i < n_reads; ++i)
    {
        ::Serial.print(read_results[i]);
        ::Serial.print(' ');
    }
    ::Serial.println();
    ::Serial.flush();

    for (unsigned int i = 0; i < n_reads; ++i)
    {
        ::Serial.print(read_times_usec[i] - start_time);
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
