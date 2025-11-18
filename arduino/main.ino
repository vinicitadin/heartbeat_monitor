#include <Arduino.h>

#define CONFIG_KY039_PIN 34
#define CONFIG_TAB_LENGTH 4
#define CONFIG_RISE_THRESHOLD 5
#define CONFIG_CALIB_OFFSET 0

float analogTab[CONFIG_TAB_LENGTH];
float analogSum;
float lastRead;
float analogAverage;
float readStartTime;
float sensorData;
float firstTime;
float secondTime;
float thirdTime;
float before;
bool rising;
int riseCount;
int numReads;
long int lastBeat;
long int currentTime;
long int ptr;

void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < CONFIG_TAB_LENGTH; i++)
    {
        analogTab[i] = 0;
    }

    analogSum = 0;
    ptr = 0;
}

void loop()
{
    numReads = 0;
    readStartTime = millis();
    analogAverage = 0.0;

    do
    {
        analogAverage = analogAverage + analogRead(CONFIG_KY039_PIN);
        numReads++;
        currentTime = millis();
    } while (currentTime < readStartTime + 20);

    analogAverage = analogAverage / numReads;

    analogSum = analogSum - analogTab[ptr];
    analogSum = analogSum + analogAverage;
    analogTab[ptr] = analogAverage;
    lastRead = analogSum / CONFIG_TAB_LENGTH;

    if (lastRead > before)
    {
        riseCount++;

        if (!rising && riseCount > CONFIG_RISE_THRESHOLD)
        {
            rising = true;
            firstTime = millis() - lastBeat;
            lastBeat = millis();
            sensorData = 60000.0 / (0.4 * firstTime + 0.3 * secondTime + 0.3 * thirdTime) + CONFIG_CALIB_OFFSET;
            thirdTime = secondTime;
            secondTime = firstTime;
        }
    }
    else
    {
        riseCount = 0;
        rising = false;
    }

    before = lastRead;
    ptr++;
    ptr = ptr % CONFIG_TAB_LENGTH;
}