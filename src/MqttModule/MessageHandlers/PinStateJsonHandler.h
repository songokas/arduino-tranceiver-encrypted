#ifndef MQTT_MODULE_MESSAGE_HANDLERS_PIN_STATE_JSON_HANDLER
#define MQTT_MODULE_MESSAGE_HANDLERS_PIN_STATE_JSON_HANDLER

#include <Arduino.h>
#include <ArduinoJson.h>
#include "IMessageHandler.h"

namespace MqttModule
{
    namespace ValueProviders { class ValueProviderFactory; }

    using ValueProviders::ValueProviderFactory;

    class PinCollection;

    namespace MessageHandlers
    {
        class PinStateJsonHandler: public IMessageHandler
        {

            public:
                PinStateJsonHandler(PinCollection & pins, ValueProviderFactory & valueProviderFactory);

                void handle(const char * channel, const char * message);

            private:
                uint8_t assignPin(const JsonObject & json);

                PinCollection & pins;
                ValueProviderFactory & valueProviderFactory;
        };
    }
}

#endif
