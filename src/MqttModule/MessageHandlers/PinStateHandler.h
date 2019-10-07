#ifndef MQTT_MODULE_MESSAGE_HANDLERS_PIN_STATE_HANDLER
#define MQTT_MODULE_MESSAGE_HANDLERS_PIN_STATE_HANDLER

#include "IMessageHandler.h"

namespace MqttModule
{
    namespace ValueProviders { class ValueProviderFactory; }

    using ValueProviders::ValueProviderFactory;

    class PinCollection;

    namespace MessageHandlers
    {
        class PinStateHandler: public IMessageHandler
        {
            public:

                PinStateHandler(PinCollection & pins, ValueProviderFactory & valueProviderFactory);


                /**
                *
                * arduino sends all its states every 10s
                * arduino listens for state changes and applies it to its pins
                *
                * send to these channels
                *
                * send heating/nodes/node132/subscribe some-channel
                *
                * send heating/nodes/node132/digital/set/5 200 - set default pin time to read and send back to 200 ms. will send back to arduino/digital/5 with data from digitalRead(5)
                * send sensors/nodes/sam/digital/set/5 1 - will digitalWrite(5, 1) and same as above
                * send lighting/nodes/sam/analog/set/3 255 - will analogWrite(3, 255) and start sending every DEFAULT_SEND_TIME to arduino/analog/3 with data from analogRead(3)
                *
                */
                void handle(const char * channel, const char * message);

            private:

                PinCollection & pins;
                ValueProviderFactory & valueProviderFactory;
        };
    }
}

#endif
