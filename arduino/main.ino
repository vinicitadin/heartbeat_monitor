#include <Arduino.h>
#include "Credentials.h"
#include <WiFi.h>
#include <PubSubClient.h>

#define CONFIG_KY039_PIN 34
#define CONFIG_BUZZER_PIN 25

#define CONFIG_TAB_LENGTH 4

#define CONFIG_RISE_THRESHOLD 5

#define CONFIG_CALIB_OFFSET 0

#define CONFIG_SENSOR_TIMER_READ 20

#define CONFIG_BUZZER_MSG_ON "1"
#define CONFIG_BUZZER_MSG_OFF "0"

#define CONFIG_WIFI_SSID WIFI_SSID
#define CONFIG_WIFI_PASSWORD WIFI_PASSWORD
#define CONFIG_WIFI_TIMEOUT_MS 10000

#define CONFIG_MQTT_SERVER MQTT_SERVER
#define CONFIG_MQTT_USER "***"
#define CONFIG_MQTT_PASSWORD "***"
#define CONFIG_MQTT_PORT 1883
#define CONFIG_MQTT_PUB_TOPIC "unesciot/heartbeat/monitor"
#define CONFIG_MQTT_SUB_TOPIC "unesciot/heartbeat/response"
#define CONFIG_MQTT_INTERVAL_TIME 20000
#define CONFIG_MQTT_RECONNECT_INTERVAL_MS 5000

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastReconnectAttempt;
float data;
bool wifiConnected;
bool mqttConnected;

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

String callbackMessage = "";
float lastSensorData = 0.0;
String lastCallbackMessage = "";

void callback(char* topic, uint8_t* payload, unsigned int length);
bool connectWiFi();
bool connectMQTT();
void publish(float data);
void checkForBeat();
float averageRead();
void setBuzzerData(String data);
bool isConnected();

void setup()
{
    Serial.begin(115200);

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

    pinMode(CONFIG_BUZZER_PIN, OUTPUT);

    WiFi.begin(CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);
    
    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < CONFIG_WIFI_TIMEOUT_MS)
    {
        delay(500);
    }
    
    wifiConnected = (WiFi.status() == WL_CONNECTED);
    
    if (wifiConnected)
    {
        client.setServer(CONFIG_MQTT_SERVER, CONFIG_MQTT_PORT);
        client.setCallback(callback);
        connectMQTT();
    }
}

void loop()
{
    lastRead = averageRead();
    checkForBeat();

    if (connectWiFi())
    {
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

        if (callbackMessage != lastCallbackMessage)
        {
            lastCallbackMessage = callbackMessage;
            setBuzzerData(callbackMessage);
        }
    }
    
    if (sensorData > 0 && sensorData != lastSensorData)
    {
        lastSensorData = sensorData;
        Serial.print(sensorData);
        Serial.println(" BPM");
        publish(sensorData);
    }
}

void publish(float data)
{
    if (!mqttConnected)
    {
        return;
    }
    
    char messageData[10];
    sprintf(messageData, "%.1f", data);
    client.publish(CONFIG_MQTT_PUB_TOPIC, messageData);
}

bool connectWiFi()
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

bool connectMQTT()
{
    if (!wifiConnected)
    {
        return false;
    }
    
    bool connected = strlen(CONFIG_MQTT_USER) > 0 ? client.connect("HeartbeatMonitor", CONFIG_MQTT_USER, CONFIG_MQTT_PASSWORD) : client.connect("HeartbeatMonitor");
    
    if (connected)
    {
        mqttConnected = true;
        client.subscribe(CONFIG_MQTT_SUB_TOPIC);
    }
    else
    {
        mqttConnected = false;
    }
    
    return mqttConnected;
}

void callback(char* topic, uint8_t* payload, unsigned int length)
{
    payload[length] = '\0';
    String receivedData = String((char *)payload);

    String topicString = String(topic);

    if (topicString.equals(CONFIG_MQTT_SUB_TOPIC)) 
    {
        callbackMessage = receivedData;
        Serial.print("Mensagem MQTT recebida: ");
        Serial.println(receivedData);
    } 
    else
    {
        Serial.print("Mensagem em tópico não esperado: ");
        Serial.println(topicString);
    }
}

bool isConnected()
{
    return wifiConnected && mqttConnected;
}

void checkForBeat()
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

float averageRead()
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

void setBuzzerData(String data)
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