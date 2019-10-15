#ifndef RADIO_ENCRYPTED_RADIO_ENCRYPTED_CONFIG_H
#define RADIO_ENCRYPTED_RADIO_ENCRYPTED_CONFIG_H

#include <Arduino.h>

// encrypted message cipher text lenth
// does not work above 100 why ?
const uint8_t MAX_LEN_ENCRYPTED_MESSAGE 100

const uint8_t KEY_LENGH {16};
const uint8_t VECTOR_LENGH {16};
const uint16_t MESH_TIMEOUT {5000};
const uint8_t RADIO_CHANNEL {112};

#endif
