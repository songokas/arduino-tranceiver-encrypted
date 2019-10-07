#ifndef RADIO_ENCRYPED_MQTT_CLIENT_H
#define RADIO_ENCRYPED_MQTT_CLIENT_H

#include <Arduino.h>
#include "SubscriberList.h"

namespace RadioEncrypted { class IEncryptedMesh; }

using RadioEncrypted::IEncryptedMesh;

namespace MqttModule
{
    class MqttMessage;

    namespace MessageHandlers { class IMessageHandler; }

    using MessageHandlers::IMessageHandler;

    class MeshMqttClient
    {
        public:
            MeshMqttClient(IEncryptedMesh & mesh, SubscriberList & subscribers, uint16_t nodeId = 0);
            bool publish(const MqttMessage & message);
            bool publish(const char * topic, unsigned long number);
            bool publish(const char * topic, const char * message);
            bool publish(const char * topic, uint16_t number);
            bool subscribe(const char * topic, IMessageHandler * handler);
            size_t loop();

        private:
            IEncryptedMesh & mesh;
            SubscriberList & subscribers;
            uint16_t toNodeId {0};

    };
}
#endif
