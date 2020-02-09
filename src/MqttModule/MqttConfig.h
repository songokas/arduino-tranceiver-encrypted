#ifndef MQTT_MODULE_MQTT_CONFIG_H
#define MQTT_MODULE_MQTT_CONFIG_H

#include <Arduino.h>

#ifndef NODE_NAME
#define NODE_NAME __DATE__
#endif

// mqtt topic
const uint8_t MAX_LEN_TOPIC {60};
// mqtt message
const uint8_t MAX_LEN_MESSAGE {40};

const uint8_t MAX_LEN_URL {150};

const char CHANNEL_SUBSCRIBE[] PROGMEM {NODE_NAME "/subscribe"};
const char CHANNEL_INFO[] PROGMEM {NODE_NAME "/%s/%d"};
const char CHANNEL_SET_JSON[] PROGMEM {NODE_NAME "/set/json"};
const char CHANNEL_KEEP_ALIVE[] PROGMEM {NODE_NAME "/keep-alive"};


const uint16_t DEFAULT_PIN_READ_TIME = 30000;
const uint8_t MAX_LEN_JSON_MESSAGE {120};

namespace MqttModule
{
    struct Pin
    {
        uint8_t id {0};
        const char * type {nullptr};
        uint16_t value {0};
        bool readOnly {true};
        uint16_t readInterval {DEFAULT_PIN_READ_TIME};
        unsigned long lastRead {0};

        
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
        Pin(uint8_t id, const char * type,  uint16_t value, bool readOnly, uint16_t readInterval)
            :id(id), type(type), value(value), readOnly(readOnly), readInterval(readInterval)
        {}
        Pin(uint8_t id, const char * type, uint16_t value, bool readOnly, uint16_t readInterval, uint16_t lastRead)
            :id(id), type(type), value(value), readOnly(readOnly), readInterval(readInterval), lastRead(lastRead)
        {}
        //
    };


}
#endif
