const unsigned int BAUD_RATE = 9600;
const unsigned int MAX_TIME_MILLISEC_WAIT_SERIAL_DATA = 10;

void setup()
{
    Serial.begin(BAUD_RATE);
    Serial.setTimeout(MAX_TIME_MILLISEC_WAIT_SERIAL_DATA);
    pinMode(LED_BUILTIN, OUTPUT);
}

void run_blink()
{
    for (unsigned int i = 0; i < 5; ++i)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
    }
}

void loop()
{
    while (Serial.available() > 0)
    {
        String message = Serial.readString();
        message.trim();

        if (message == "test")
        {
            run_blink();
        }
        else if (message == "exit")
        {
            Serial.end();
            break;
        }
        else
        {
            Serial.println(message);
        }
    }
}
