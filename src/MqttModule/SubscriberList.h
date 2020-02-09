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
        public:
            uint32_t topicCrc {0};
            uint8_t handlerSize {0};

            virtual void addHandler(IMessageHandler * handler, uint16_t node) = 0;
            virtual bool hasHandler(IMessageHandler * handler) const = 0;
            virtual bool hasNode(uint16_t node) const = 0;
            virtual bool addNode(uint16_t node) = 0;
            virtual IMessageHandler * getHandler(uint8_t i) = 0;

    };

    template<uint8_t maxHandlers, uint8_t maxNodes>
    struct StaticSubscriber: public Subscriber
    {
        public:
            void addHandler(IMessageHandler * handler, uint16_t node) { handlers[handlerSize] = handler; nodes[handlerSize] = node; handlerSize++; }
            bool addNode(uint16_t node) { for (uint16_t & current: nodes) { if (current == 0) { current = node; return true; }} return false; }
            bool hasHandler(IMessageHandler * handler) const { for (IMessageHandler * current: handlers) { if (current == handler) { return true; }} return false;}
            bool hasNode(uint16_t node) const { for (uint16_t current: nodes) { if (current == node) { return true; }} return false; }
        
            IMessageHandler * getHandler(uint8_t i) { return i >= maxHandlers ? handlers[maxHandlers - 1] : handlers[i]; }
        
        private:
            IMessageHandler * handlers[maxHandlers] {nullptr};
            uint16_t nodes[maxNodes] {0};

    };

    class SubscriberList
    {
        public:
            bool hasSubscribed(const char * topic);
            size_t call(const MqttMessage & message);
            bool add(const char * topic, IMessageHandler * handler, uint16_t nodeId);
            size_t countSubscribers() const;
            size_t countHandlers(const char * topic);
            bool hasHandler(const char * topic, IMessageHandler * handler);

        protected:
            virtual uint8_t getSubscriberArrLength() const = 0;
            virtual Subscriber & getSubscriberByIndex(uint8_t i) = 0;
            virtual const Subscriber & getSubscriberByIndex(uint8_t i) const = 0;

        private:
            bool hasNode(const Subscriber & subscriber, uint16_t nodeId);
            Subscriber * getSubscribed(const char * topic);
            uint8_t subscriberSize {0};
   
    };

    template<uint8_t maxSubscribers, uint8_t maxHandlers, uint8_t maxNodes>
    class StaticSubscriberList: public SubscriberList
    {
        protected:
            uint8_t getSubscriberArrLength() const { return maxSubscribers; }
            Subscriber & getSubscriberByIndex(uint8_t i) { return i >= maxSubscribers ? subscribers[maxSubscribers - 1] : subscribers[i]; }
            const Subscriber & getSubscriberByIndex(uint8_t i) const { return i >= maxSubscribers ? subscribers[maxSubscribers - 1] : subscribers[i]; };
        private:
            StaticSubscriber<maxHandlers, maxNodes> subscribers[maxSubscribers];
    };
}

#endif
