#include <Arduino.h>
#include "Heartbeat.h"
#include "Comm.h"
#include "configs.h"

Heartbeat heartbeat = Heartbeat();
Comm comm = Comm();

float sensorData = 0.0;
float lastSensorData = 0.0;

void setup()
{
  Serial.begin(115200);
  heartbeat.begin();
  comm.begin();
}

void loop()
{
  heartbeat.loop();
  comm.loop();

  sensorData = heartbeat.getSensorData();
  if (sensorData != lastSensorData)
  {
    lastSensorData = sensorData;
    Serial.print(sensorData);
    Serial.println(" BPM");
    comm.publish(sensorData);
  }
}