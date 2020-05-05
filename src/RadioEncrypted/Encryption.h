#ifndef RADIO_ENCRYPTED_ENCRYPTION_H
#define RADIO_ENCRYPTED_ENCRYPTION_H

#include <Arduino.h>
#include "RadioEncryptedConfig.h"

class Acorn128;

namespace RadioEncrypted
{
    namespace Entropy { class IEntropy; }

    using Entropy::IEntropy;

    class Encryption
    {
        public:
            Encryption(Acorn128 & cipher, const char * key, IEntropy & entropy);
            bool encrypt(const void * data, size_t len, const void * authData, size_t authLen, EncryptedMessage & message);
            bool decrypt(const void * authData, size_t authLen, const EncryptedMessage & message, void * data, size_t len);
            bool randomBytes(uint8_t * bytes, size_t len);
        private:
            bool setupCipher(const uint8_t (&initVector)[ENCRYPTION_VECTOR_LENGTH], const void * authDat, size_t authLen);
            Acorn128 & cipher;
            const char * key;
            IEntropy & entropy;

    };
}


#endif
