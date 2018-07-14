#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "user_config.h"

#define ON "ON"
#define OFF "OFF"
#define SUBSCRIBE_STATE_TOPIC "ha/switch1"
#define PUBLISH_STATE_TOPIC "ha/switch1/state"
#define LEDPin 13
#define IS_DEBUG_MODE true

bool switchState = false;

WiFiClient espClient;
PubSubClient client(espClient);

void turnOnSwitchWithNotify();
void turnOffSwitchWithNotify();
void publishState();
boolean shouldTurnOn(char *topic, byte *payload, unsigned int length);
boolean shouldTurnOff(char *topic, byte *payload, unsigned int length);
void onMessageReceived(char *topic, byte *payload, unsigned int length);
void reconnect();
void setup_wifi();
void setupPins();
void debbugLog(char *massage);
void debbugLog(int massage);

void setup()
{
  Serial.begin(115200);
  delay(500);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(onMessageReceived);
  setupPins();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();
}

void onMessageReceived(char *topic, byte *payload, unsigned int length)
{
  if (shouldTurnOn(topic, payload, length))
  {
    turnOnSwitchWithNotify();
  }
  else if (shouldTurnOff(topic, payload, length))
  {
    turnOffSwitchWithNotify();
  }
}

void reconnect()
{
  while (!client.connected())
  {
    debbugLog("Attempting MQTT connection...\r\n");
    if (client.connect("arduinoClient", MQTT_USER, MQTT_PASSWORD))
    {
      debbugLog("connected\r\n");
      publishState();
      client.subscribe(SUBSCRIBE_STATE_TOPIC);
    }
    else
    {
      debbugLog("failed with state: ");
      debbugLog(client.state());
      debbugLog("try again in 4 seconds\r\n");
      delay(4000);
    }
  }
}

void publishState()
{
  if (switchState)
  {
    client.publish(PUBLISH_STATE_TOPIC, ON);
    debbugLog("ON\r\n");
  }
  else
  {
    client.publish(PUBLISH_STATE_TOPIC, OFF);
    debbugLog("OFF\r\n");
  }
}

void turnOnSwitchWithNotify()
{
  digitalWrite(LEDPin, LOW);
  switchState = true;
  publishState();
}

void turnOffSwitchWithNotify()
{
  digitalWrite(LEDPin, LOW);
  switchState = true;
  publishState();
}

void setupPins()
{
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
}

boolean shouldTurnOn(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String strTopic = String((char *)topic);
  return strTopic == SUBSCRIBE_STATE_TOPIC && String((char *)payload) == ON;
}

boolean shouldTurnOff(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String strTopic = String((char *)topic);
  return strTopic == SUBSCRIBE_STATE_TOPIC && String((char *)payload) == OFF;
}

void setup_wifi()
{
  debbugLog("Connecting to : ");
  debbugLog(MY_SSID);

  WiFi.begin(MY_SSID, PASSWORD);

  delay(1500);

  debbugLog("\r\nConnecting to WiFi..\r\n");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    debbugLog(".");
  }

  debbugLog("\r\nWiFi connected\r\n");
  debbugLog("IP address: ");
  debbugLog(WiFi.localIP());

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, HIGH);
}

void debbugLog(char *massage)
{
  if (IS_DEBUG_MODE)
  {
    Serial.print(massage);
  }
}

void debbugLog(int massage)
{
  if (IS_DEBUG_MODE)
  {
    Serial.print(massage);
  }
}