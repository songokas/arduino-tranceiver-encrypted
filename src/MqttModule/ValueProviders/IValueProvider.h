#ifndef MQTT_MODULE_VALUE_PROVIDERS_IVALUE_PROVIDER_H
#define MQTT_MODULE_VALUE_PROVIDERS_IVALUE_PROVIDER_H

#include <ArduinoJson.h>

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        class IValueProvider
        {
            public:
                virtual const char * getPinType() const = 0;
                virtual bool formatMessage(char * message, size_t len, const Pin & pin) = 0;
                virtual bool addJson(JsonDocument & json, const Pin & pin) = 0;
                virtual bool apply(const Pin & pin) = 0;
        };
    }
}

#endif
