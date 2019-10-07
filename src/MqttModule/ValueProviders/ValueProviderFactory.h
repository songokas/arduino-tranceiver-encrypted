#ifndef MQTT_MODULE_VALUE_PROVIDERS_VALUE_PROVIDER_FACTORY_H
#define MQTT_MODULE_VALUE_PROVIDERS_VALUE_PROVIDER_FACTORY_H

#include "IValueProvider.h"
#include "../MqttConfig.h"

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        struct ProviderArray
        {
            uint8_t pin {0};
            uint16_t type {0};
            IValueProvider * provider {nullptr};
        };

        class ValueProviderFactory: public IValueProvider
        {
            public:
                const char * getPinType() const;

                bool formatMessage(char * message, size_t len, const Pin & pin);

                bool apply(const Pin & pin);

                void addByType(uint16_t type, IValueProvider * provider, uint8_t pin = 0);

                uint16_t getMatchingPinType(const char * type, size_t len) const;

                const char * getMatchingTopicType(const Pin & pin) const;

            private:
                
                IValueProvider * getProvider(const Pin & pin) const;

                uint8_t getFreeIndex() const;
                
                ProviderArray providers[MAX_PROVIDERS];
        };
    }
}

#endif
