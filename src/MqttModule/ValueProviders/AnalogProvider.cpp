#include <Arduino.h>

#include "../MqttConfig.h"
#include "AnalogProvider.h"

using MqttModule::ValueProviders::AnalogProvider;
using MqttModule::Pin;

bool AnalogProvider::formatMessage(char * message, size_t len, const Pin & pin)
{
    snprintf_P(message, len, PSTR("%d"), analogRead(pin.id));
    return true;
}

bool AnalogProvider::apply(const Pin & pin)
{   
    analogWrite(pin.id, pin.value);
    return true;
}

const char * AnalogProvider::getPinType() const
{
    return "analog";
}
