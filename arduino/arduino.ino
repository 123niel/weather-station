#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

#include "settings.h"

Adafruit_BME280 bme;

const unsigned int intervall = 900000;

WiFiClient wifiClient;
Adafruit_MQTT_Client mqttClient(&wifiClient, MQTT_HOST, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqttClient, "weather/temperature");
Adafruit_MQTT_Publish pressure = Adafruit_MQTT_Publish(&mqttClient, "weather/pressure");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqttClient, "weather/humidity");
Adafruit_MQTT_Publish light = Adafruit_MQTT_Publish(&mqttClient, "weather/light");
Adafruit_MQTT_Publish voltage = Adafruit_MQTT_Publish(&mqttClient, "weather/voltage");
void connectMqtt();

void setup()
{
  Serial.begin(9600);

  connectWifi();
  connectI2C();
  connectMqtt();

  temperature.publish(bme.readTemperature());
  pressure.publish(bme.readPressure());
  humidity.publish(bme.readHumidity());

  delay(200);
  deepSleep();
}

void connectI2C()
{
  while (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    delay(100);
  }
}

void connectWifi()
{
  WiFi.forceSleepWake();
  delay(1);
  WiFi.mode(WIFI_STA);

  Serial.println("Connecting Wifi");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
    delay(100);

  Serial.printf("IP adress: %s\n", WiFi.localIP().toString().c_str());
}

void connectMqtt()
{
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 10;
  while (mqttClient.connect())
  {
    Serial.print(" retrying ");
    delay(100);
  }
  Serial.println("MQTT Connected!");
}

void deepSleep()
{
  Serial.println("deep sleep");
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  ESP.deepSleep(intervall * 1000);
}

void loop()
{
}
