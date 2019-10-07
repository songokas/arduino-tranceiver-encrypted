#ifndef RADIO_ENCRYPTED_ENTROPY_ESP_ADAPTER_H
#define RADIO_ENCRYPTED_ENTROPY_ESP_ADAPTER_H

#include "IEntropy.h"

class ESP8266TrueRandomClass;

namespace RadioEncrypted
{
    namespace Entropy
    {
        class EspRandomAdapter: public IEntropy
        {
            public:
                EspRandomAdapter(ESP8266TrueRandomClass & generator);
                uint8_t randomByte();
            private:
                ESP8266TrueRandomClass & generator;
        };
    }
}

#endif
