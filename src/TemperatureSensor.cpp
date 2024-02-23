#include "TemperatureSensor.h";

TemperatureSensor::TemperatureSensor(int bus) : oneWire(bus), sensors(&oneWire) { 
}

void TemperatureSensor::init() {
  sensors.begin();
}

float TemperatureSensor::getTemperature() {
  sensors.requestTemperatures(); 
  return sensors.getTempCByIndex(0);
}