#ifndef __COMM__H__
#define __COMM__H__

#include <Wifi.h>
#include <PubSubClient.h>
#include "configs.h"

class Comm
{
private:
    WiFiClient espClient;
    PubSubClient client;
    unsigned long lastReconnectAttempt;
    float data;
    bool wifiConnected;
    bool mqttConnected;
    String callbackMessage;

    bool connectWiFi();
    bool connectMQTT();
public:
    Comm();
    void begin();
    void loop();
    void publish(float data);
    void callback(char* topic, uint8_t* payload, unsigned int length);
    bool isConnected();
    String getCallbackMessage() { return callbackMessage; }
};

#endif  //!__COMM__H__