#include <Arduino.h>

#include "CommonModule/MacroHelper.h"
#include "SubscribeHandler.h"
#include "../MeshMqttClient.h"

using MqttModule::MessageHandlers::SubscribeHandler;

SubscribeHandler::SubscribeHandler(MeshMqttClient & client, IMessageHandler & handler): client(client), handler(handler)
{}

void SubscribeHandler::handle(const char * topic, const char * message)
{
    if (!client.subscribe(message, &handler)) {
        error("Failed to register subscription");
    } else {
        debug("Subscribed for: %s", message);
    }
}
