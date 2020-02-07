#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "../MqttConfig.h"
#include "DallasTemperatureProvider.h"

using MqttModule::ValueProviders::DallasTemperatureProvider;
using MqttModule::Pin;

DallasTemperatureProvider::DallasTemperatureProvider(DallasTemperature & sensor): sensor(sensor)
{

}

const char * DallasTemperatureProvider::getPinType() const
{
    return "temperature";
}

bool DallasTemperatureProvider::formatMessage(char * message, size_t len, const Pin & pin)
{
    if (!started) {
        sensor.begin();
        started = true;
    }
    sensor.requestTemperatures();
    float temp = sensor.getTempCByIndex(0);
    snprintf_P(message, len, PSTR("%u"), (uint16_t)(100 * temp));
    return true;
}

bool DallasTemperatureProvider::apply(const Pin & pin)
{
    return false;
}
