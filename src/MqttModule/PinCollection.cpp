#include <Arduino.h>

#include "PinCollection.h"
#include "MqttConfig.h"

using MqttModule::PinCollection;

PinCollection::PinCollection(Pin * pins, uint8_t len)
    :pins(pins), len(len)
{}

bool PinCollection::set(const Pin & pin)
{
    for (uint8_t i = 0; i < len; i++) {
        if (pins[i].id == pin.id) {
            Pin & p = pins[i];
            p.value = pin.value;
            p.readInterval = p.readInterval;
            return true;
        }
    }
    return false;
}

bool PinCollection::hasAvailablePin(uint8_t id) const
{
    for (uint8_t i = 0; i < len; i++) {
        if (pins[i].id == id) {
            return true;
        }
    }
    return false;
}

bool PinCollection::isReadOnly(uint8_t id) const
{
    for (uint8_t i = 0; i < len; i++) {
        if (pins[i].id == id) {
            Pin & pin = pins[i];
            return pin.readOnly;
        }
    }
    return false;
}

