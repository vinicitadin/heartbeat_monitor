#include <Arduino.h>
#include "Heartbeat.h"
#include "configs.h"

Heartbeat heartbeat = Heartbeat();

float sensorData = 0.0;

void setup()
{
  Serial.begin(115200);
  heartbeat.begin();
}

void loop()
{
  heartbeat.loop();
  sensorData = heartbeat.getSensorData();
  Serial.print(sensorData);
  Serial.println(" BPM");
}