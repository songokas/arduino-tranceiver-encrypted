#ifndef RADIO_ENCRYPTED_ENTROPY_H
#define RADIO_ENCRYPTED_ENTROPY_H

#include <Arduino.h>

namespace RadioEncrypted
{
    namespace Entropy
    {
        class IEntropy
        {
            public:
                virtual uint8_t randomByte() = 0;
        };
    }
}

#endif
