const unsigned int MAX_SIZE_MSG = 12;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    while (Serial.available() > 0)
    {
        static char msg[MAX_SIZE_MSG];
        static unsigned int idx = 0;

        char inc = Serial.read();

        if ((inc != '\n') and (idx < MAX_SIZE_MSG - 1))
        {
            msg[idx] = inc;
            idx++;
        }
        else
        {
            msg[idx] = '\0';
            Serial.println(msg);
            idx = 0;
        }
    }
}
