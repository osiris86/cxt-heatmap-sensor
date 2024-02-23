#include <ESP8266WiFi.h>
#include "Wireless.h"

Wireless::Wireless(String lHostname, String lSsid, String lPass) {
  hostname = lHostname;
  ssid = lSsid;
  pass = lPass;
}

void Wireless::connect() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  WiFi.hostname(hostname);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connected!");
}

void Wireless::keepAlive() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Checking wifi");
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(10);
    }
    Serial.println("connected");
  }
}