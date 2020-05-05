#ifndef MQTT_MODULE_MQTT_MESSAGE_H
#define MQTT_MODULE_MQTT_MESSAGE_H

#include <Arduino.h>
#include "CommonModule/MacroHelper.h"
#include "MqttConfig.h"

namespace MqttModule
{

#pragma pack (1)
    struct MqttMessage
    {
        char topic[MQTT_MAX_LEN_TOPIC] {0};
        char message[MQTT_MAX_LEN_MESSAGE] {0};

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

    struct MessageQueueItem
    {
        uint16_t node {0};
        MqttMessage message;
        bool initialized {false};
        uint8_t failedToSend {0};
    };

    struct Pin
    {
        uint8_t id {0};
        const char * type {nullptr};
        uint16_t value {0};
        bool readOnly {true};
        bool changed {false};
        //uint16_t readInterval {DEFAULT_PIN_READ_TIME};
        //unsigned long lastRead {0};

        
        //c++11
        //
        Pin()
        {}

        Pin(uint8_t id)
            :id(id)
        {}
        Pin(uint8_t id, const char * type)
            :id(id), type(type)
        {}
        Pin(uint8_t id, const char * type, uint16_t value)
            :id(id), type(type), value(value)
        {}
        Pin(uint8_t id, const char * type,  uint16_t value, bool readOnly)
            :id(id), type(type), value(value), readOnly(readOnly)
        {}
        Pin(uint8_t id, const char * type,  uint16_t value, bool readOnly, bool changed)
            :id(id), type(type), value(value), readOnly(readOnly), changed(changed)
        {}
        // Pin(uint8_t id, const char * type,  uint16_t value, bool readOnly, uint16_t readInterval)
            // :id(id), type(type), value(value), readOnly(readOnly), readInterval(readInterval)
        // {}
        //Pin(uint8_t id, const char * type, uint16_t value, bool readOnly, uint16_t readInterval, uint16_t lastRead)
        //    :id(id), type(type), value(value), readOnly(readOnly), readInterval(readInterval), lastRead(lastRead)
        //{}
        //
    };
}

#endif
