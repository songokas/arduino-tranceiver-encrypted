#include <Arduino.h>
#include <CRC32.h>

#include "CommonModule/MacroHelper.h"
#include "MqttMessage.h"
#include "MessageHandlers/IMessageHandler.h"
#include "SubscriberList.h"

using MqttModule::Subscriber;
using MqttModule::SubscriberList;

SubscriberList::SubscriberList(Subscriber * subscribers, uint8_t subscriberArrLength)
    : subscribers(subscribers), subscriberArrLength(subscriberArrLength)
{}

Subscriber * SubscriberList::getSubscribed(const char * topic)
{
    uint32_t crc = CRC32::calculate(topic, strlen(topic));
    for (uint8_t i = 0; i < subscriberSize; i++) {
        if (subscribers[i].topicCrc == crc) {
            return &subscribers[i];
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
        if (subscriberSize >= subscriberArrLength) {
            return false;
        }

        uint32_t crc = CRC32::calculate(topic, strlen(topic));
        subscribers[subscriberSize].topicCrc = crc;
        subscribers[subscriberSize].handlers[0] = handler;
        subscribers[subscriberSize].nodes[0] = nodeId;
        subscribers[subscriberSize].handlerSize++;
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
    
    for (uint8_t i = 0; i < sub->nodeArrLength; i++) {
        if (sub->nodes[i] == 0) {
            sub->nodes[i] = nodeId;
            return true;
        }
    }
    return false;
}

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
    for (uint8_t i = 0; i < subscriber.nodeArrLength; i++) {
        if (subscriber.nodes[i] == nodeId) {
            return true;
        }
    }
   return false;
}

const Subscriber * SubscriberList::getSubscriber(const char * topic)
{
    return getSubscribed(topic);
}
