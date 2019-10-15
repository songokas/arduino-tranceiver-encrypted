#ifndef RADIO_ENCRYPTED_MQTT_MESSAGE_H
#define RADIO_ENCRYPTED_MQTT_MESSAGE_H

#include <Arduino.h>
#include "CommonModule/MacroHelper.h"
#include "MqttConfig.h"

namespace MqttModule
{

#pragma pack (1)
    struct MqttMessage
    {
        char topic[MAX_LEN_TOPIC] {0};
        char message[MAX_LEN_MESSAGE] {0};

        MqttMessage () {}
        MqttMessage(const char * topic) {
            snprintf_P(this->topic, sizeof(this->topic), PSTR("%s"), topic);
        }
        MqttMessage(const char * topic, const char * message) {
            snprintf_P(this->topic, COUNT_OF(this->topic), PSTR("%s"), topic);
            snprintf_P(this->message, COUNT_OF(this->message), PSTR("%s"), message);
        }
    };
#pragma pack (0)

    enum class MessageType:uint8_t
    {
        All = 0,
        Publish,
        Subscribe,
    };

}

#endif
