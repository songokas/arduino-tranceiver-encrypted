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
        uint32_t topicCrc {0};
        //char topic[MAX_LEN_TOPIC] {0};
        IMessageHandler * handlers [MAX_HANDLERS_PER_TOPIC] {nullptr};
        size_t handlerSize {0};
        uint16_t nodes[MAX_NODES_PER_TOPIC] {0};
        
        Subscriber() {}

        //c++11 requires constuctor
        Subscriber(uint32_t topicCrc, IMessageHandler * handler, uint16_t node): topicCrc(topicCrc), handlerSize(1)
        {
        handlers[0] = handler;
        nodes[0] = node;
        }
    };


    class SubscriberList
    {
        public:
            bool hasSubscribed(const char * topic);
            size_t call(const MqttMessage & message);
            bool add(const char * topic, IMessageHandler * handler, uint16_t nodeId);
            //bool add(const Subscriber & subscriber);
            size_t countSubscribers() const;
            size_t countHandlers(const char * topic);
            bool hasHandler(const char * topic, IMessageHandler * handler);
            const Subscriber * getSubscriber(const char * topic);

        private:
            bool hasNode(const Subscriber & subscriber, uint16_t nodeId);
            Subscriber * getSubscribed(const char * topic);
            size_t subscriberSize {0};
            Subscriber subscribers[MAX_SUBSCRIBERS];
            
    };
}

#endif
