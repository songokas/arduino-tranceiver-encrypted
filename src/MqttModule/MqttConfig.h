#ifndef MQTT_MODULE_MQTT_CONFIG_H
#define MQTT_MODULE_MQTT_CONFIG_H

#include <Arduino.h>

#ifndef AVAILABLE_PINS
#define AVAILABLE_PINS {{2, 0}}
#endif

#ifndef MAX_NODES_PER_TOPIC
#define MAX_NODES_PER_TOPIC 2
#endif
#ifndef MAX_SUBSCRIBERS
#define MAX_SUBSCRIBERS 10
#endif

#ifndef NODE_NAME
#define NODE_NAME __DATE__
#endif

#ifndef MAX_PROVIDERS
#define MAX_PROVIDERS 4
#endif

const char CHANNEL_SUBSCRIBE[] PROGMEM {NODE_NAME "/subscribe"};
const char CHANNEL_INFO[] PROGMEM {NODE_NAME "/%s/%d"};
const char CHANNEL_SET_JSON[] PROGMEM {NODE_NAME "/set/json"};
const char CHANNEL_KEEP_ALIVE[] PROGMEM {NODE_NAME "/keep-alive"};

const uint16_t DEFAULT_PIN_READ_TIME = 30000;
const uint8_t MAX_HANDLERS_PER_TOPIC {2};
const uint8_t MAX_LEN_JSON_MESSAGE {120};

namespace MqttModule
{

    enum class PinType:uint16_t
    {
        Digital = 0,
        Analog
    };

    struct Pin
    {
        uint8_t id {0};
        uint16_t type {0};
        uint16_t value {0};
        bool readOnly {false};
        uint16_t readInterval {DEFAULT_PIN_READ_TIME};
        unsigned long lastRead {0};

        
        //c++11
        //
        Pin()
        {}

        Pin(uint8_t id)
            :id(id)
        {}
        Pin(uint8_t id, uint16_t type)
            :id(id), type(type)
        {}
        Pin(uint8_t id, uint16_t type, uint16_t value)
            :id(id), type(type), value(value)
        {}
        Pin(uint8_t id, uint16_t type,  uint16_t value, bool readOnly)
            :id(id), type(type), value(value), readOnly(readOnly)
        {}
        Pin(uint8_t id, uint16_t type,  uint16_t value, bool readOnly, uint16_t readInterval)
            :id(id), type(type), value(value), readOnly(readOnly), readInterval(readInterval)
        {}
        Pin(uint8_t id, uint16_t type, uint16_t value, bool readOnly, uint16_t readInterval, uint16_t lastRead)
            :id(id), type(type), value(value), readOnly(readOnly), readInterval(readInterval), lastRead(lastRead)
        {}
        //
    };


}
#endif
