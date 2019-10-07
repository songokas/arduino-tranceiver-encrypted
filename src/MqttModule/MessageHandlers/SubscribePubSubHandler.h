#ifndef MQTT_MODULE_MESSAGE_HANDLERS_SUBSCRIBE_HANDLER
#define MQTT_MODULE_MESSAGE_HANDLERS_SUBSCRIBE_HANDLER

#include "IMessageHandler.h"

class PubSubClient;

namespace MqttModule
{
    class MeshMqttClient;
    class SubscriberList;

    namespace MessageHandlers
    {
        class SubscribePubSubHandler: public IMessageHandler
        {
            public:

                SubscribePubSubHandler(PubSubClient & client, SubscriberList & subscribers, IMessageHandler & handler);

                /**
                * listens to a channel sent
                */
                void handle(const char * topic, const char * message);

            private:
                PubSubClient & client;
                SubscriberList & subscribers;
                IMessageHandler & handler;
        };
    }
}

#endif
