const unsigned int BAUD_RATE = 9600;
const unsigned int MAX_TIME_MILLISEC_WAIT_SERIAL_DATA = 10;

void setup()
{
    Serial.begin(BAUD_RATE);
    Serial.setTimeout(MAX_TIME_MILLISEC_WAIT_SERIAL_DATA);
    pinMode(LED_BUILTIN, OUTPUT);
}

void run_connection_test()
{
    Serial.println("Hello from InoDAQ2. I should blink 5 times!");
    Serial.flush();

    for (unsigned int i = 0; i < 5; ++i)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }
}

void exit_program()
{
    Serial.println("Closing connection. Goodbye!");
    Serial.flush();
    Serial.end();
}

void handle_unknown_command(const String &command)
{
    Serial.println("Unknown command: " + command);
    Serial.flush();
}

void loop()
{
    while (Serial.available() > 0)
    {
        String command = Serial.readString();
        command.trim();

        if (command == "hello")
        {
            run_connection_test();
        }
        else if (command == "exit")
        {
            exit_program();
            break;
        }
        else
        {
            handle_unknown_command(command);
        }
    }
}
