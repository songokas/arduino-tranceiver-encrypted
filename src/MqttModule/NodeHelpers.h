#ifndef MQTT_MODULE_NODE_HELPERS_H
#define MQTT_MODULE_NODE_HELPERS_H

#include <Arduino.h>
#include "MqttConfig.h"

using MqttModule::Pin;

inline uint16_t findPosFromEnd(const char * str, uint16_t length, char c)
{
    for (uint16_t i = length; i > 0; i--) {
        if (str[i] == c) {
            return i;
        }
    }
    return 0;
}



#endif
