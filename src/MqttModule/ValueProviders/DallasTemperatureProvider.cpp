#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "../MqttConfig.h"
#include "../MqttMessage.h"
#include "DallasTemperatureProvider.h"

using MqttModule::ValueProviders::DallasTemperatureProvider;
using MqttModule::Pin;
using MqttModule::ValueProviders::DallasSensor;

DallasTemperatureProvider::DallasTemperatureProvider(DallasSensor * sensors, uint8_t sensorArrLength): sensors(sensors), sensorArrLength(sensorArrLength)
{
}

const char * DallasTemperatureProvider::getPinType() const
{
    return "temperature";
}

bool DallasTemperatureProvider::formatMessage(char * message, size_t len, const Pin & pin)
{
    if (!hasSensor(pin.id)) {
        return false;
    }
    snprintf_P(message, len, PSTR("%u"), getValue(pin.id));
    return true;
}

bool DallasTemperatureProvider::addJson(JsonDocument & json, const Pin & pin)
{
    if (!hasSensor(pin.id)) {
        return false;
    }
    json[F("temperature")] = getValue(pin.id);
    return true;
}

bool DallasTemperatureProvider::apply(const Pin & pin)
{
    return false;
}

bool DallasTemperatureProvider::hasSensor(uint8_t pin)
{
    return getSensor(pin) != nullptr;
}

DallasSensor * DallasTemperatureProvider::getSensor(uint8_t pin)
{
    for (uint8_t i = 0; i < sensorArrLength; i++) {
        if (sensors[i].pin == pin) {
            return &sensors[i];
        }
    }
    return nullptr;
}

uint16_t DallasTemperatureProvider::getValue(uint8_t pin)
{
    if (!hasSensor(pin)) {
        return 0;
    }

    DallasSensor * reader = getSensor(pin);
    if (reader->started) {
        reader->sensor.begin();
        reader->started = true;
    }

    reader->sensor.requestTemperatures();
    float temp = reader->sensor.getTempCByIndex(0);
    return (uint16_t)(100 * temp); 
}
