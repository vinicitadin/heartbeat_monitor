#include <Arduino.h>
#include "Heartbeat.h"
#include "Buzzer.h"
#include "Comm.h"
#include "configs.h"

Heartbeat heartbeat = Heartbeat();
Buzzer buzzer = Buzzer();
Comm comm = Comm();

float sensorData = 0.0;
float lastSensorData = 0.0;
String callbackMessage = "";
String lastCallbackMessage = "";

void setup()
{
  Serial.begin(115200);
  heartbeat.begin();
  buzzer.begin();
  comm.begin();
}

void loop()
{
  heartbeat.loop();
  comm.loop();

  callbackMessage = comm.getCallbackMessage();
  if (callbackMessage != lastCallbackMessage)
  {
    lastCallbackMessage = callbackMessage;
    buzzer.setData(callbackMessage);
  }

  sensorData = heartbeat.getSensorData();
  if (sensorData != lastSensorData)
  {
    lastSensorData = sensorData;
    Serial.print(sensorData);
    Serial.println(" BPM");
    comm.publish(sensorData);
  }
}