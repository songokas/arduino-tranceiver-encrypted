#ifndef RADIO_ENCRYPTED_ENTROPY_AVR_ENTROPY_ADAPTER_H
#define RADIO_ENCRYPTED_ENTROPY_AVR_ENTROPY_ADAPTER_H

#include "IEntropy.h"

class EntropyClass;

namespace RadioEncrypted
{
    namespace Entropy
    {
        class AvrEntropyAdapter: public IEntropy
        {
            public:
                AvrEntropyAdapter(EntropyClass & generator);
                uint8_t randomByte();
            private:
                EntropyClass & generator;
        };
    }
}

#endif
