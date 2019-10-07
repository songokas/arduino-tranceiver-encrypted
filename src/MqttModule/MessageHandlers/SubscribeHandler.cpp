#include <Arduino.h>
#include <Streaming.h>

#include "SubscribeHandler.h"
#include "../MeshMqttClient.h"

using MqttModule::MessageHandlers::SubscribeHandler;

SubscribeHandler::SubscribeHandler(MeshMqttClient & client, IMessageHandler & handler): client(client), handler(handler)
{}

void SubscribeHandler::handle(const char * topic, const char * message)
{
    if (!client.subscribe(message, &handler)) {
        Serial << F("Failed to register subscription") << endl;
    } else {
        Serial << F("Subscribed for: ") << message << endl;
    }
}
