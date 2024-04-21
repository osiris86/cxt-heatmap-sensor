#include <WiFiClientSecure.h>
#include <time.h>
#include <PubSubClient.h>
#include "secrets.h"
#include "MQTT.h"
#include "TemperatureSensor.h"
#include "Wireless.h"

#define DEVICE_ID "1"
#define INTERVAL 300000
#define ONE_WIRE_BUS 0

TemperatureSensor tempSensor(ONE_WIRE_BUS);
MQTT mqttClient(DEVICE_ID);
Wireless wifi("cxt-heatmap-sensor-" + String(DEVICE_ID), SECRET_SSID, SECRET_PASS);

time_t now;
unsigned long lastMillis = 0;

void setTime() {
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
}

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println();
  
  wifi.connect();

  setTime();

  mqttClient.init();
}

void loop() {
  now = time(nullptr);

  wifi.keepAlive();
  mqttClient.keepAlive();

  if (millis() - lastMillis > INTERVAL) {
    lastMillis = millis();

    mqttClient.report(tempSensor.getTemperature());
  }
}