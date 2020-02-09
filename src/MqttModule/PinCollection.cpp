#include <Arduino.h>

#include "PinCollection.h"
#include "MqttConfig.h"

using MqttModule::PinCollection;

bool PinCollection::set(const Pin & pin)
{
    Pin * current = getPin(pin.id);
    if (current) {
        current->value = pin.value;
        current->readInterval = pin.readInterval;
        return true;
    }
    return false;
}

bool PinCollection::hasAvailablePin(uint8_t id) const
{
    return getPin(id) != nullptr;
}

bool PinCollection::isReadOnly(uint8_t id) const
{
    auto current = getPin(id);
    if (current) {
        return current->readOnly;
    }
    return false;
}


