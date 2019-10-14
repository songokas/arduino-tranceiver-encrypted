#ifndef RADIO_ENCRYPTED_ENTROPY_ANALOG_SIGNAL_ENTROPY_H
#define RADIO_ENCRYPTED_ENTROPY_ANALOG_SIGNAL_ENTROPY_H

#include "IEntropy.h"

class EntropyClass;

namespace RadioEncrypted
{
    namespace Entropy
    {
        class AnalogSignalEntropy: public IEntropy
        {
            public:
                AnalogSignalEntropy(uint8_t pin, uint16_t seed);
                uint8_t randomByte();
            private:
                uint8_t pin {0};
        };
    }
}

#endif
