// esp cant load EntropyClass
#ifdef __AVR__

#include <Entropy.h>
#include "AvrEntropyAdapter.h"

using RadioEncrypted::Entropy::AvrEntropyAdapter;

AvrEntropyAdapter::AvrEntropyAdapter(EntropyClass & generator)
    :generator(generator)
{}

uint8_t AvrEntropyAdapter::randomByte()
{
    return generator.randomByte();
}

#endif
