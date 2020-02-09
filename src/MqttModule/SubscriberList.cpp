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
    for (uint8_t i = 0; i < getSubscriberArrLength(); i++) {
        auto & subscriber = getSubscriberByIndex(i);
        if (subscriber.topicCrc == crc) {
            return &subscriber;
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
    uint8_t i {0};
    while (sub && i < sub->handlerSize) {
        auto handler = sub->getHandler(i);
        if (handler) {
            handler->handle(message.topic, message.message);
        }
        i++;
    }
    return i;
}

bool SubscriberList::add(const char * topic, IMessageHandler * handler, uint16_t nodeId)
{

    Subscriber * sub = getSubscribed(topic);
    if (sub == nullptr) {
        if (subscriberSize >= getSubscriberArrLength()) {
            return false;
        }

        uint32_t crc = CRC32::calculate(topic, strlen(topic));
        auto & subscriber = getSubscriberByIndex(subscriberSize);
        subscriber.topicCrc = crc;
        subscriber.addHandler(handler, nodeId);
        subscriberSize++;
        return true;
    }

    bool handlerAdded = false;
    if (!sub->hasHandler(handler)) {
        sub->addHandler(handler, nodeId);
        handlerAdded = true;
    }

    if (sub->hasNode(nodeId)) {
        return handlerAdded;
    } 
    return sub->addNode(nodeId);
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
    Subscriber * sub = getSubscribed(topic);
    return sub && sub->hasHandler(handler);
}

