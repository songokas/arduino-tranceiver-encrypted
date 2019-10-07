#ifndef RADIO_ENCRYPED_IMESSAGE_HANDLER
#define RADIO_ENCRYPED_IMESSAGE_HANDLER

#include <Arduino.h>

namespace MqttModule
{
    namespace MessageHandlers
    {
        class IMessageHandler
        {
            public:
                virtual void handle(const char * channel, const char * message) = 0;
        };
    }
}

#endif 
