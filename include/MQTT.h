#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class MQTT {
  private:
    BearSSL::WiFiClientSecure net;
    PubSubClient client;
    char hostname[21];
    char* deviceId;
  public:
    MQTT(char* lDeviceId);
    void init();
    void connect();
    void keepAlive();
    void report(float temperature);
};
