#include <Arduino.h>

#include "PinCollection.h"
#include "MqttConfig.h"

using MqttModule::PinCollection;

PinCollection::PinCollection(Pin * pins, uint8_t pinArrLength)
    :pins(pins), pinArrLength(pinArrLength)
{}

bool PinCollection::set(const Pin & pin)
{
    for (uint8_t i = 0; i < pinArrLength; i++) {
        if (pins[i].id == pin.id) {
            pins[i].value = pin.value;
            pins[i].readInterval = pin.readInterval;
            return true;
        }
    }
    return false;
}

bool PinCollection::hasAvailablePin(uint8_t id) const
{
    for (uint8_t i = 0; i < pinArrLength; i++) {
        if (pins[i].id == id) {
            return true;
        }
    }
    return false;
}

bool PinCollection::isReadOnly(uint8_t id) const
{
    for (uint8_t i = 0; i < pinArrLength; i++) {
        if (pins[i].id == id) {
            return pins[i].readOnly;
        }
    }
    return false;
}

