#include "Heartbeat.h"

Heartbeat::Heartbeat()
{
}

void Heartbeat::begin()
{
    for (int i = 0; i < CONFIG_TAB_LENGTH; i++)
    {
        analogTab[i] = 0;
    }

    analogSum = 0;
    ptr = 0;
    lastRead = 0.0;
    analogAverage = 0.0;
    readStartTime = 0.0;
    sensorData = 0.0;
    firstTime = 0.0;
    secondTime = 0.0;
    thirdTime = 0.0;
    before = 0.0;
    rising = false;
    riseCount = 0;
    numReads = 0;
    lastBeat = 0;
    currentTime = 0;
}

void Heartbeat::loop()
{
    lastRead = averageRead();
    checkForBeat();
}

void Heartbeat::checkForBeat()
{
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

float Heartbeat::averageRead()
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

    return analogSum / CONFIG_TAB_LENGTH;
}
