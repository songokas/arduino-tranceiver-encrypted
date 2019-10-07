#include <Arduino.h>
#include <CRC32.h>

#include "CommonModule/MacroHelper.h"
#include "MqttMessage.h"
#include "MessageHandlers/IMessageHandler.h"
#include "SubscriberList.h"

using MqttModule::Subscriber;
using MqttModule::SubscriberList;

Subscriber * SubscriberList::getSubscribed(const char * topic)
{
    uint32_t crc = CRC32::calculate(topic, strlen(topic));
    for (auto & sub: subscribers) {
        if (sub.topicCrc == crc) {
            return &sub;
        }
    }
    return nullptr;
}

bool SubscriberList::hasSubscribed(const char * topic)
{
    return getSubscribed(topic) != nullptr;
}

size_t SubscriberList::call(const MqttMessage & message)
{
    Subscriber * sub = getSubscribed(message.topic);
    size_t i {0};
    while (sub && i < sub->handlerSize) {
        if (sub->handlers[i]) {
            sub->handlers[i]->handle(message.topic, message.message);
        }
        i++;
    }
    return i;
}

bool SubscriberList::add(const char * topic, IMessageHandler * handler, uint16_t nodeId)
{

    Subscriber * sub = getSubscribed(topic);
    if (sub == nullptr) {
        if (subscriberSize >= COUNT_OF(subscribers)) {
            return false;
        }

        uint32_t crc = CRC32::calculate(topic, strlen(topic));
        subscribers[subscriberSize] = Subscriber(crc, handler, nodeId);
        subscriberSize++;
        return true;
    }

    bool handlerAdded = false;
    if (!hasHandler(topic, handler)) {
        sub->handlers[sub->handlerSize] = handler;
        sub->handlerSize++;
        handlerAdded = true;
    }

    if (hasNode(*sub, nodeId)) {
        return handlerAdded;
    }
    
    for (auto & node: sub->nodes) {
        if (node == 0) {
            node = nodeId;
            return true;
        }
    }
    return false;
}

/*bool SubscriberList::add(const Subscriber & subscriber)
{
    return add(subscriber.topic, subscriber.handlers[0], subscriber.nodes[0]);
}*/

size_t SubscriberList::countSubscribers() const
{
    return subscriberSize;
}

size_t SubscriberList::countHandlers(const char * topic)
{
    Subscriber * sub = getSubscribed(topic);
    if (!sub) {
        return 0;
    }
    return sub->handlerSize;
}

bool SubscriberList::hasHandler(const char * topic, IMessageHandler * handler)
{
    size_t i {0};
    Subscriber * sub = getSubscribed(topic);
    while (sub && i < sub->handlerSize) {
        if (sub->handlers[i] == handler) {
            return true;
        }
        i++;
    }
    return false;
}

bool SubscriberList::hasNode(const Subscriber & subscriber, uint16_t nodeId)
{
   for (auto node: subscriber.nodes) {
        if (node == nodeId) {
            return true;
        }
    }
   return false;
}

const Subscriber * SubscriberList::getSubscriber(const char * topic)
{
    return getSubscribed(topic);
}
