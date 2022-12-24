const unsigned int BAUD_RATE = 9600;
const unsigned int MAX_TIME_MILLISEC_WAIT_SERIAL_DATA = 10;

void setup()
{
    Serial.begin(BAUD_RATE);
    Serial.setTimeout(MAX_TIME_MILLISEC_WAIT_SERIAL_DATA);
}

void do_something(const String &message)
{
    Serial.println(message);
}

void loop()
{
    while (Serial.available() > 0)
    {
        String message = Serial.readString();
        message.trim();

        if (message == "exit")
        {
            Serial.println("exiting...");
        }
        else if (message == "read")
        {
            do_something(message);
        }
        else
        {
            Serial.println(message);
        }
    }
}
