#ifndef MQTT_MODULE_VALUE_PROVIDERS_DALLAS_TEMPERATURE_PROVIDER_H
#define MQTT_MODULE_VALUE_PROVIDERS_DALLAS_TEMPERATURE_PROVIDER_H

#include "IValueProvider.h"

class DallasTemperature;

namespace MqttModule
{
    struct Pin;

    namespace ValueProviders
    {
        struct DallasSensor
        {
            DallasTemperature sensor;
            uint8_t pin {0};
            bool started {false};

            DallasSensor(DallasTemperature sensor, uint8_t pin): sensor(sensor), pin(pin)
            {}
        };

        class DallasTemperatureProvider: public IValueProvider
        {
            public:
                DallasTemperatureProvider(DallasSensor * sensors, uint8_t sensorArrLength);

                const char * getPinType() const;
                bool formatMessage(char * message, size_t len, const Pin & pin);
                bool addJson(JsonDocument & json, const Pin & pin);
                bool apply(const Pin & pin);
                bool hasSensor(uint8_t pin);
            private:
                DallasSensor * getSensor(uint8_t pin);
                uint16_t getValue(uint8_t pin);
                DallasSensor * sensors;
                uint8_t sensorArrLength {0};
             
        };
    }
}

#endif
