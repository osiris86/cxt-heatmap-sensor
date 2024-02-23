#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureSensor {
  private:
    OneWire oneWire;
    DallasTemperature sensors;
  public:
    TemperatureSensor(int bus);
    void init();
    float getTemperature();
};
