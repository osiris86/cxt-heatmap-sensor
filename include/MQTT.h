#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTT {
  private:
    WiFiClient net;
    PubSubClient client;
    char hostname[21];
    String deviceId;
  public:
    MQTT(String lDeviceId);
    void init();
    void connect();
    void keepAlive();
    void report(float temperature);
};
