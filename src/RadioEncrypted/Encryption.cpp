#include <Arduino.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "Entropy/IEntropy.h"
#include "Encryption.h"

using RadioEncrypted::Encryption;
using RadioEncrypted::Entropy::IEntropy;

Encryption::Encryption(Acorn128 & cipher, const char * key, IEntropy & entropy)
    : cipher(cipher), key(key), entropy(entropy)
{
}

bool Encryption::setupCipher(const uint8_t (&initVector)[ENCRYPTION_VECTOR_LENGTH], const void * authData, size_t authLen)
{
    cipher.clear();
    if (!cipher.setKey((uint8_t*)key, strlen(key))) {
        error("Encryption key must be 16 bytes long. Provided %d", strlen(key) + 1);
        return false;
    }
    if (!cipher.setIV(initVector, COUNT_OF(initVector))) {
        error("Encryption setIV failed");
        return false;
    }

    if (authLen > 0) {
        cipher.addAuthData(authData, authLen);
    }
    return true;

}

bool Encryption::encrypt(const void * data, size_t len, const void * authData, size_t authLen, EncryptedMessage & message)
{
    bool result = setupCipher(message.nonce, authData, authLen);
    if (!result) {
        return result;
    }

    cipher.encrypt(message.cipherText, (const uint8_t *)data, MIN(len, COUNT_OF(message.cipherText)));
    cipher.computeTag(message.tag, COUNT_OF(message.tag));

    return true;
}

bool Encryption::decrypt(const void * authData, size_t authLen, const EncryptedMessage & message, void * data, size_t len)
{
    bool result = setupCipher(message.nonce, authData, authLen);
    if (!result) {
        return result;
    }

    cipher.decrypt((uint8_t *)data, message.cipherText, MIN(len, COUNT_OF(message.cipherText)));

    if (!cipher.checkTag(message.tag, COUNT_OF(message.tag))) {
        warning("Tag check failed ... ");
        return false;
    }

    return true;
}

bool Encryption::randomBytes(uint8_t * bytes, size_t len)
{
    size_t i = 0;
    while (i < len) {
        bytes[i] = entropy.randomByte();
        i++;
    }
    return true;
}

