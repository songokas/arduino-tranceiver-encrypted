#ifndef MQTT_MODULE_VALUE_PROVIDERS_DALLAS_TEMPERATURE_PROVIDER_H
#define MQTT_MODULE_VALUE_PROVIDERS_DALLAS_TEMPERATURE_PROVIDER_H

#include "IValueProvider.h"

class DallasTemperature;

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        class DallasTemperatureProvider: public IValueProvider
        {
            public:
                DallasTemperatureProvider(DallasTemperature & sensor);

                const char * getPinType() const;
                bool formatMessage(char * message, size_t len, const Pin & pin);
                bool apply(const Pin & pin);
            private:
                bool started {false};
                DallasTemperature & sensor;
             
        };
    }
}

#endif
