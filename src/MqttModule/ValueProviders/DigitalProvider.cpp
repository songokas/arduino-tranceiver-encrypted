#include <Arduino.h>
#include "../MqttConfig.h"

#include "DigitalProvider.h"

using MqttModule::ValueProviders::DigitalProvider;
using MqttModule::Pin;

bool DigitalProvider::formatMessage(char * message, size_t len, const Pin & pin)
{
    snprintf_P(message, len, PSTR("%d"), digitalRead(pin.id));
    return true;
}

bool DigitalProvider::apply(const Pin & pin)
{   
    digitalWrite(pin.id, pin.value);
    return true;
}

const char * DigitalProvider::getPinType() const
{
    return "digital";
}
