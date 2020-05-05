#include <Arduino.h>
#include <RF24Network.h>

#include "CommonModule/MacroHelper.h"
#include "RadioEncrypted/IEncryptedMesh.h"
#include "MqttMessage.h"
#include "MeshMqttClient.h"

using MqttModule::MeshMqttClient;
using MqttModule::MqttMessage;
using RadioEncrypted::IEncryptedMesh;

MeshMqttClient::MeshMqttClient(IEncryptedMesh & mesh, SubscriberList & subscribers, uint16_t toNodeId)
    :mesh(mesh), subscribers(subscribers), toNodeId(toNodeId)
{}

bool MeshMqttClient::publish(const MqttMessage & message)
{
    return mesh.send(&message, sizeof(message), (uint8_t)MessageType::Publish, toNodeId);
}

bool MeshMqttClient::publish(const char * topic, uint16_t value)
{
    MqttMessage message(topic);
    snprintf_P(message.message, sizeof(message.message), PSTR("%d"), value);
    return publish(message);
}

bool MeshMqttClient::publish(const char * topic, unsigned long value)
{
    MqttMessage message(topic);
    snprintf_P(message.message, sizeof(message.message), PSTR("%lu"), value);
    return publish(message);
}

bool MeshMqttClient::publish(const char * topic, const char * msg)
{
    MqttMessage message(topic, msg);
    return publish(message);
}

bool MeshMqttClient::subscribe(const char * topic, IMessageHandler * handler)
{
    subscribers.add(topic, handler, mesh.getNodeId());
    return true;
    //MqttMessage message(topic);
    //return mesh.send(&message, sizeof(message), (uint8_t)MessageType::Subscribe, toNodeId);
}

size_t MeshMqttClient::loop()
{
    size_t i = 0;
    while (mesh.isAvailable()) {
        MqttMessage message;
        RF24NetworkHeader header;
        if (mesh.receive(&message, sizeof(message), (uint8_t)MessageType::All, header)) {
            message.topic[COUNT_OF(message.topic) - 1] = '\0';
            message.message[COUNT_OF(message.message) - 1] = '\0';
            debug("Mqtt received: %s", message.message);
            subscribers.call(message);
            i++;
        } else {
            error("Mqtt failed to read message");
        }

    }
    return i;
}

