#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "user_config.h"

#define ON "ON"
#define OFF "OFF"
#define IS_DEBUG_MODE true

WiFiClient espClient;
PubSubClient client(espClient);
int SwitchedPin = 12, LEDPin = 13;
bool HassSwitch = false;
String switch1;
String strTopic;
String strPayload;

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
      client.subscribe("ha/#");
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
  if (HassSwitch)
  {
    client.publish("ha/switch1/state", ON);
    debbugLog("ON\r\n");
  }
  else
  {
    client.publish("ha/switch1/state", OFF);
    debbugLog("OFF\r\n");
  }
}

void turnOnSwitchWithNotify()
{
  digitalWrite(SwitchedPin, HIGH);
  digitalWrite(LEDPin, LOW);
  HassSwitch = true;
  publishState();
}

void turnOffSwitchWithNotify()
{
  digitalWrite(SwitchedPin, HIGH);
  digitalWrite(LEDPin, LOW);
  HassSwitch = true;
  publishState();
}

void setupPins()
{
  pinMode(SwitchedPin, OUTPUT);
  digitalWrite(SwitchedPin, LOW);
}

boolean shouldTurnOn(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  strTopic = String((char *)topic);
  return strTopic == "ha/switch1/action" && String((char *)payload) == ON;
}

boolean shouldTurnOff(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  strTopic = String((char *)topic);
  return strTopic == "ha/switch1/action" && String((char *)payload) == OFF;
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