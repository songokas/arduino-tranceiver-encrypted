#ifndef RADIO_ENCRYPTED_RADIO_ENCRYPTED_CONFIG_H
#define RADIO_ENCRYPTED_RADIO_ENCRYPTED_CONFIG_H

#include <Arduino.h>

// mqtt topic
const uint8_t MAX_LEN_TOPIC {70};
// mqtt message
const uint8_t MAX_LEN_MESSAGE {80};

// encrypted message cipher text lenth
const uint8_t MAX_LEN_ENCRYPTED_MESSAGE (MAX_LEN_TOPIC + MAX_LEN_MESSAGE);

const uint8_t KEY_LENGH {16};
const uint8_t VECTOR_LENGH {16};
const uint16_t MESH_TIMEOUT {5000};
const uint8_t RADIO_CHANNEL {112};

#endif
