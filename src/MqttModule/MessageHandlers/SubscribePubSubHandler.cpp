#include <Arduino.h>
#include <PubSubClient.h>

#include "CommonModule/MacroHelper.h"
#include "SubscribePubSubHandler.h"
#include "../SubscriberList.h"

using MqttModule::MessageHandlers::SubscribePubSubHandler;
using MqttModule::SubscriberList;
using MqttModule::MessageHandlers::IMessageHandler;

SubscribePubSubHandler::SubscribePubSubHandler(PubSubClient & client, SubscriberList & subscribers, IMessageHandler & handler)
    : client(client), subscribers(subscribers), handler(handler)
{}

void SubscribePubSubHandler::handle(const char * topic, const char * message)
{
    if (client.subscribe(message)) {
        subscribers.add(message, &handler, (uint16_t)0);
        debug("Subscribed for: %s", message);
    } else {
        debug("Failed to register subscription");
    }
}
