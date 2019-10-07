#ifndef MQTT_MODULE_VALUE_PROVIDERS_ANALOG_PROVIDER_H
#define MQTT_MODULE_VALUE_PROVIDERS_ANALOG_PROVIDER_H

#include "IValueProvider.h"

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        class AnalogProvider: public IValueProvider
        {
            public:
                const char * getPinType() const;

                bool formatMessage(char * message, size_t len, const Pin & pin);

                bool apply(const Pin & pin);
        };
    }
}

#endif
