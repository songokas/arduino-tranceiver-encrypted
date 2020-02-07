#include <Arduino.h>
#include <Streaming.h>
#include "../MqttConfig.h"
#include "ValueProviderFactory.h"

using MqttModule::ValueProviders::ValueProviderFactory;
using MqttModule::Pin;
using MqttModule::ValueProviders::IValueProvider;

ValueProviderFactory::ValueProviderFactory(IValueProvider ** providers, uint8_t providerArrLength)
    :providers(providers), providerArrLength(providerArrLength)
{}

const char * ValueProviderFactory::getMatchingTopicType(const Pin & pin) const
{
    IValueProvider * provider = getProvider(pin);
    if (!provider) {
        return getPinType();
    }
    return provider->getPinType();
}

bool ValueProviderFactory::formatMessage(char * message, size_t len, const Pin & pin)
{
    IValueProvider * provider = getProvider(pin);
    if (!provider) {
        return false;
    }
    return provider->formatMessage(message, len, pin);
}

bool ValueProviderFactory::apply(const Pin & pin)
{
    IValueProvider * provider = getProvider(pin);
    if (!provider) {
        return false;
    }
    return provider->apply(pin);
}


const char * ValueProviderFactory::getMatchingPinType(const char * type, size_t length) const
{
    for (uint8_t i = 0; i < providerArrLength; i++) {
        if (strncmp(providers[i]->getPinType(), type, length) == 0) {
            return providers[i]->getPinType();
        }
    }
    return nullptr;
}

const char * ValueProviderFactory::getPinType() const
{
    return "none";
}

IValueProvider * ValueProviderFactory::getProvider(const Pin & pin) const
{
    if (pin.type == nullptr) {
        return nullptr;
    }
    for (uint8_t i = 0; i < providerArrLength; i++) {
        if (strcmp(providers[i]->getPinType(), pin.type) == 0) {
            return providers[i];
        }
    }
    return nullptr;
}
