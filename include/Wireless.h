#include <Arduino.h>

class Wireless {
  private:
    String hostname;
    String ssid;
    String pass;
  public:
    Wireless(String lHostname, String lSsid, String lPass);
    void connect();
    void keepAlive();
};