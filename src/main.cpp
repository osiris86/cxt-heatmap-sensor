#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <PubSubClient.h>
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MQTT.h"

#define DEVICE_ID "1"
#define INTERVAL 3000

const int oneWireBus = 0;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
MQTT mqttClient(DEVICE_ID);

time_t now;
unsigned long lastMillis = 0;

char hostname[21];

void setup()
{
  //analogReference(INTERNAL);
  /*String strHostname = "cxt-heatmap-sensor-" + String(DEVICE_ID);
  int hostname_lenght = strHostname.length() + 1;
  strHostname.toCharArray(hostname, hostname_lenght);*/
  
  Serial.begin(9600);

  sensors.begin();
  Serial.println();
  Serial.println();
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(SECRET_SSID);
  WiFi.hostname(hostname);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connected!");

  Serial.print("Setting time using SNTP");
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < 1510592825) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  mqttClient.init();
}

void loop()
{
  now = time(nullptr);
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Checking wifi");
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      WiFi.begin(SECRET_SSID, SECRET_PASS);
      Serial.print(".");
      delay(10);
    }
    Serial.println("connected");
  }
  else
  {
    mqttClient.keepAlive();
  }

  if (millis() - lastMillis > INTERVAL) {
    lastMillis = millis();

    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);

    mqttClient.report(temperatureC);
  }
}