#ifndef RADIO_ENCRYPTED_SUBSCRIBER_LIST_H
#define RADIO_ENCRYPTED_SUBSCRIBER_LIST_H

#include <Arduino.h>
#include "MqttConfig.h"

namespace MqttModule
{

    namespace MessageHandlers { class IMessageHandler; }
    
    class MqttMessage;
    using MessageHandlers::IMessageHandler;

    struct Subscriber
    {
        IMessageHandler ** handlers {nullptr};
        uint8_t handlerArrLength {0};
        uint16_t * nodes {nullptr};
        uint8_t nodeArrLength {0};
        
        uint32_t topicCrc {0};
        uint8_t handlerSize {0};
        
        Subscriber(IMessageHandler ** handlers, uint8_t handlerArrLength, uint16_t * nodes, uint8_t nodeArrLength)
            : handlers(handlers), handlerArrLength(handlerArrLength), nodes(nodes), nodeArrLength(nodeArrLength)
        {}
    };


    class SubscriberList
    {
        public:
            SubscriberList(Subscriber * subscribers, uint8_t subscriberArrLength);
            bool hasSubscribed(const char * topic);
            size_t call(const MqttMessage & message);
            bool add(const char * topic, IMessageHandler * handler, uint16_t nodeId);
            size_t countSubscribers() const;
            size_t countHandlers(const char * topic);
            bool hasHandler(const char * topic, IMessageHandler * handler);
            const Subscriber * getSubscriber(const char * topic);

        private:
            bool hasNode(const Subscriber & subscriber, uint16_t nodeId);

            Subscriber * getSubscribed(const char * topic);
            Subscriber * subscribers;
            uint8_t subscriberArrLength {0};
            uint8_t subscriberSize {0};
    };
}

#endif
