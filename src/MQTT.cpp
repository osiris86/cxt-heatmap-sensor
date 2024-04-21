#include "MQTT.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "secrets.h"

MQTT::MQTT(String lDeviceId) : client(net) { 
  deviceId = lDeviceId;
}

void MQTT::init() {
  BearSSL::X509List cert(digicert);
  net.setTrustAnchors(&cert);

  client.setServer(SECRET_MQTT_SERVER, SECRET_MQTT_PORT);
  connect();
}

void MQTT::connect() {
  while (!client.connected()) {
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

void MQTT::keepAlive() {
  if (!client.connected()) {
    connect();
  } else {
    client.loop();
  }
}

void MQTT::report(float temperatureC) {
  String jsonData = String("{\"id\":\""+deviceId+"\", \"temp\": \""+String(temperatureC)+"\"}");
  Serial.println(jsonData);
  int data_len = jsonData.length() + 1;
  char payload[data_len];
  jsonData.toCharArray(payload, data_len);
  client.publish("cxt/temperature", payload, false);
}