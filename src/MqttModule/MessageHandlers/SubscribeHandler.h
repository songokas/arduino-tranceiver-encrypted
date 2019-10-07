#ifndef MQTT_MODULE_MESSAGE_HANDLERS_SUBSCRIBE_HANDLER
#define MQTT_MODULE_MESSAGE_HANDLERS_SUBSCRIBE_HANDLER

#include "IMessageHandler.h"

namespace MqttModule
{
    class MeshMqttClient;

    namespace MessageHandlers
    {
        class SubscribeHandler: public IMessageHandler
        {
            public:

                SubscribeHandler(MeshMqttClient & client, IMessageHandler & handler);

                /**
                * listens to a channel sent
                */
                void handle(const char * topic, const char * message);

            private:
                MeshMqttClient & client;
                IMessageHandler & handler;
        };
    }
}

#endif
