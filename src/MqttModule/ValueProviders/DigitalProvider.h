#ifndef MQTT_MODULE_VALUE_PROVIDERS_DIGITAL_PROVIDER_H
#define MQTT_MODULE_VALUE_PROVIDERS_DIGITAL_PROVIDER_H

#include "IValueProvider.h"

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        class DigitalProvider: public IValueProvider
        {
            public:
                const char * getPinType() const;
                bool formatMessage(char * message, size_t len, const Pin & pin);
                bool apply(const Pin & pin);
                bool addJson(JsonDocument & json, const Pin & pin);
        };
    }
}

#endif
