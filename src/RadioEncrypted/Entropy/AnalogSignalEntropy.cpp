#include <Arduino.h>

#include "AnalogSignalEntropy.h"

using RadioEncrypted::Entropy::AnalogSignalEntropy;

AnalogSignalEntropy::AnalogSignalEntropy(uint8_t pin, uint16_t seed) : pin(pin)
{
    randomSeed(random(2147483647L) ^ (pin + seed));
}

uint8_t AnalogSignalEntropy::randomByte()
{
    uint8_t times = 8;
    while (times) {
        randomSeed(random(2147483647L) ^ analogRead(pin));
        times--;
    }
    return (uint8_t)random(255);
}
