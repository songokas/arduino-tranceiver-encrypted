#include <Arduino.h>
#include "../MqttConfig.h"
#include "ValueProviderFactory.h"

using MqttModule::ValueProviders::ValueProviderFactory;
using MqttModule::Pin;
using MqttModule::ValueProviders::IValueProvider;

const char * ValueProviderFactory::getMatchingTopicType(const Pin & pin) const
{
    IValueProvider * provider = getProvider(pin);
    if (!provider) {
        return getPinType();
    }
    return provider->getPinType();
}

bool ValueProviderFactory::formatMessage(char * topic, size_t len, const Pin & pin)
{
    IValueProvider * provider = getProvider(pin);
    if (!provider) {
        return false;
    }
    return provider->formatMessage(topic, len, pin);
}

bool ValueProviderFactory::apply(const Pin & pin)
{
    IValueProvider * provider = getProvider(pin);
    if (!provider) {
        return false;
    }
    return provider->apply(pin);
}

void ValueProviderFactory::addByType(uint16_t type, IValueProvider * provider, uint8_t pin)
{
    //c++11 issue
    //providers[getFreeIndex()] = {pin, type, provider};
    uint8_t index = getFreeIndex();
    providers[index].pin = pin;
    providers[index].type = type;
    providers[index].provider = provider;
}

uint16_t ValueProviderFactory::getMatchingPinType(const char * type, size_t len) const
{
    for (const auto & q: providers) {
        if (q.provider == nullptr) {
            continue;
        }
        if (strncmp(q.provider->getPinType(), type, len)) {
            return q.type;
        }
    }
    return 0;
}

const char * ValueProviderFactory::getPinType() const
{
    return "none";
}

IValueProvider * ValueProviderFactory::getProvider(const Pin & pin) const
{
    for (const auto & q: providers) {
        if (q.pin == pin.id) {
            return q.provider;
        }
    }
    for (const auto & q: providers) {
        if (q.type == pin.type) {
            return q.provider;
        }
    }
    return nullptr;
}

uint8_t ValueProviderFactory::getFreeIndex() const
{
    uint8_t index = 0;
    for (const auto & q: providers) {
        if (q.pin == 0 && q.type == 0) {
            return index;
        }
        index++;
    }
    return index;
}
