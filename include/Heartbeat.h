#ifndef __HEARTBEAT__H__
#define __HEARTBEAT__H__

#include <Arduino.h>
#include "configs.h"

class Heartbeat
{
private:
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
public:
    Heartbeat();
    void begin();
    void loop();
    void checkForBeat();
    float averageRead();
    float getSensorData() { return sensorData; }
};

#endif  //!__HEARTBEAT__H__