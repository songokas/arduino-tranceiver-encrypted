#ifndef MQTT_MODULE_VALUE_PROVIDERS_VALUE_PROVIDER_FACTORY_H
#define MQTT_MODULE_VALUE_PROVIDERS_VALUE_PROVIDER_FACTORY_H

#include "IValueProvider.h"
#include "../MqttConfig.h"

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        class ValueProviderFactory: public IValueProvider
        {
            public:
                ValueProviderFactory(IValueProvider ** providers, uint8_t providerArrLength);

                const char * getPinType() const;

                bool formatMessage(char * message, size_t len, const Pin & pin);

                bool apply(const Pin & pin);

                const char * getMatchingPinType(const char * type, size_t len) const;
                const char * getMatchingTopicType(const Pin & pin) const;

            private:
                
                IValueProvider * getProvider(const Pin & pin) const;

                IValueProvider ** providers;
                uint8_t providerArrLength {0};
        };
    }
}

#endif
