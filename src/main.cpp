#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <PubSubClient.h>
#include "secrets.h"

#define DEVICE_ID "1"
#define INTERVAL 10000

BearSSL::WiFiClientSecure net;
PubSubClient client(net);

time_t now;
unsigned long lastMillis = 0;

char hostname[21];

int sensorPin = A0; 

void mqtt_connect()
{
  while (!client.connected()) {
    Serial.print("Time: ");
    Serial.print(ctime(&now));
    Serial.print("MQTT connecting ... ");
    if (client.connect(hostname, SECRET_MQTT_USER, SECRET_MQTT_PASS)) {
      Serial.println("connected.");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println(". Try again in 5 seconds.");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

void setup()
{
  String strHostname = "cxt-heatmap-sensor-" + String(DEVICE_ID);
  int hostname_lenght = strHostname.length() + 1;
  strHostname.toCharArray(hostname, hostname_lenght);
  Serial.begin(9600);
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

  BearSSL::X509List cert(digicert);
  net.setTrustAnchors(&cert);

  client.setServer(SECRET_MQTT_SERVER, SECRET_MQTT_PORT);
  mqtt_connect();
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
    if (!client.connected())
    {
      mqtt_connect();
    }
    else
    {
      client.loop();
    }
  }

  if (millis() - lastMillis > INTERVAL) {
    lastMillis = millis();
    Serial.println("Reporting temperature");
    int reading = analogRead(sensorPin);         // Den Signalausgang des Temperatursensors lesen 
    float voltage = reading * 5.0;               // Umwandlung der Messung in mV
    voltage /= 1024.0; 
  
    Serial.print("Gemessene Spannung: ");
    Serial.print(voltage);                       // Ausgabe der gemessenen Spannung in mV
    Serial.print(" mV;\t"); 
    float temperatureC = (voltage - 0.5) * 100 ; // Umrechnung der Spannung in C°
    Serial.print("Gemessene Temperatur: ");
    Serial.print(temperatureC);                  // Ausgabe der berechneten Temperatur in °C
    Serial.println(" °C");
    String jsonData = String("{\"id\":\""+String(DEVICE_ID)+"\", \"temp\": \""+String(temperatureC)+"\"}");
    int data_len = jsonData.length() + 1;
    char payload[data_len];
    jsonData.toCharArray(payload, data_len);
    client.publish("cxtTest", payload, false);
  }
}