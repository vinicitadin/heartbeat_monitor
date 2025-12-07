#include "Buzzer.h"
#include "configs.h"

Buzzer::Buzzer()
{
}

void Buzzer::begin()
{
    pinMode(CONFIG_BUZZER_PIN, OUTPUT);
}

void Buzzer::setData(String data)
{
    if (data == CONFIG_BUZZER_MSG_ON)
    {
        digitalWrite(CONFIG_BUZZER_PIN, HIGH);
    }
    else if (data == CONFIG_BUZZER_MSG_OFF)
    {
        digitalWrite(CONFIG_BUZZER_PIN, LOW);
    }
}