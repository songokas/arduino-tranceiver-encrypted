// due too Arduino.mk automatic loading this is compiled which it should not
#ifndef __AVR__

#include <ESP8266TrueRandom.h>
#include "EspRandomAdapter.h"

using RadioEncrypted::Entropy::EspRandomAdapter;

EspRandomAdapter::EspRandomAdapter(ESP8266TrueRandomClass & generator)
    :generator(generator)
{}

uint8_t EspRandomAdapter::randomByte()
{
    return generator.randomByte();
}

#endif
