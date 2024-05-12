#include <Arduino.h>

class Wireless {
  private:
    String hostname;
    String ssid;
  public:
    Wireless(String lHostname, String lSsid);
    void connect();
    void keepAlive();
};