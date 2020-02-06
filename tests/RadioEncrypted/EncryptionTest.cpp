#include "fakeit.hpp"
#include "catch.hpp"

#include <Arduino.h>
#include <Crypto.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "RadioEncrypted/Entropy/IEntropy.h"
#include "RadioEncrypted/Encryption.h"

using RadioEncrypted::Entropy::IEntropy;
using RadioEncrypted::Encryption;
using RadioEncrypted::EncryptedMessage;

using namespace fakeit;

TEST_CASE( "encryption tests", "[encryption]" ) {

    Acorn128 cipher;
    Mock<IEntropy> entropyMock;
    When(Method(entropyMock, randomByte)).AlwaysReturn(1);

    IEntropy & entropy = entropyMock.get();
    const char * key = "long long longpa";
    Encryption encryption(cipher, key, entropy);

    SECTION( "test random bytes" ) {

        uint8_t bytes[16] {0};
        for (auto b: bytes) {
            REQUIRE(b == 0);
        }
        encryption.randomBytes(bytes, 16);
        for (auto b: bytes) {
            REQUIRE(b != 0);
        }
    }

    SECTION ( "test encryption" ) {
        EncryptedMessage msg {"random random n"};

        const char testData[] = "test";
        const char authData[] = "test auth data 123";
        
        REQUIRE(msg.cipherText[0] == 0);
        REQUIRE(msg.tag[0] == 0);
        REQUIRE(encryption.encrypt((void*)testData, COUNT_OF(testData), (void*)authData, COUNT_OF(authData), msg) == true);
        REQUIRE(msg.cipherText[0] != 0);
        REQUIRE(msg.tag[0] != 0);

        SECTION ( "test decryption" ) {
            char decryptedData[COUNT_OF(testData)];
            bool result = encryption.decrypt((void*)authData, COUNT_OF(authData), msg, decryptedData, COUNT_OF(decryptedData));
            INFO("decrypted" << decryptedData);
            REQUIRE(result == true);
            REQUIRE(strcmp(testData, decryptedData) == 0);
        }
    }

    SECTION ( "test incorrect decryption" ) {
        EncryptedMessage msg {"random random n"};

        const char * testData = "test data 123";
        const char * authData = "test auth data 123";

        REQUIRE(encryption.encrypt((void*)testData, strlen(testData), (void*)authData, strlen(authData), msg) == true);

        SECTION ( "test decryption with wrong auth" ) {
            char decryptedData[100];
            REQUIRE(encryption.decrypt((void*)"unknown", 7, msg, decryptedData, 100) == false);
            REQUIRE(strcmp(testData, decryptedData) != 0);
        }

        SECTION ( "test decryption with different key " ) {

            const char * key2 {"random random k4"};
            Encryption encryption2(cipher, key2, entropy);
            char decryptedData[100];
            REQUIRE(encryption2.decrypt((void*)authData, strlen(authData), msg, decryptedData, 100) == false);
            REQUIRE(strcmp(testData, decryptedData) != 0);
        }

        SECTION ( "test decryption with different nonce" ) {

            EncryptedMessage msg {"test nonce encr"};
            char decryptedData[100];
            REQUIRE(encryption.decrypt((void*)authData, strlen(authData), msg, decryptedData, 100) == false);
            REQUIRE(strcmp(testData, decryptedData) != 0);
        }
    }
}
