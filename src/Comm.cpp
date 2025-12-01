#include "Comm.h"

Comm::Comm() : client(espClient)
{
    lastReconnectAttempt = 0;
    wifiConnected = false;
    mqttConnected = false;
}

void Comm::begin()
{    
    WiFi.begin(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);
    client.setServer(CONFIG_MQTT_SERVER, CONFIG_MQTT_PORT);
    
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < CONFIG_WIFI_TIMEOUT_MS)
    {
        delay(500);
    }
    
    wifiConnected = (WiFi.status() == WL_CONNECTED);
    
    if (wifiConnected)
    {
        connectMQTT();
    }
}

bool Comm::connectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        wifiConnected = true;
        return true;
    }
    
    if (wifiConnected)
    {
        WiFi.reconnect();
    }
    
    wifiConnected = (WiFi.status() == WL_CONNECTED);
    return wifiConnected;
}

bool Comm::connectMQTT()
{
    if (!wifiConnected)
    {
        return false;
    }
    
    bool connected = strlen(CONFIG_MQTT_USER) > 0 ? client.connect("HeartbeatMonitor", CONFIG_MQTT_USER, CONFIG_MQTT_PASSWORD) : client.connect("HeartbeatMonitor");
    
    if (connected)
    {
        mqttConnected = true;
    }
    else
    {
        mqttConnected = false;
    }
    
    return mqttConnected;
}

void Comm::loop()
{
    if (!connectWiFi())
    {
        return;
    }
    
    if (!client.connected())
    {
        mqttConnected = false;
        unsigned long now = millis();
        if (now - lastReconnectAttempt > CONFIG_MQTT_RECONNECT_INTERVAL_MS)
        {
            lastReconnectAttempt = now;
            connectMQTT();
        }
    }
    else
    {
        client.loop();
    }
}

void Comm::publish(float data)
{
    if (!mqttConnected)
    {
        return;
    }
    
    char messageData[10];
    sprintf(messageData, "%.1f", data);
    client.publish(CONFIG_MQTT_PUB_TOPIC, messageData);
}

bool Comm::isConnected()
{
    return wifiConnected && mqttConnected;
}