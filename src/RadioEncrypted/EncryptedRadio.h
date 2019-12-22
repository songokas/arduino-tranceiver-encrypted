#ifndef RADIO_ENCRYPTED_ENCRYPTED_RADIO_H
#define RADIO_ENCRYPTED_ENCRYPTED_RADIO_H

#include <Arduino.h>
#include "IEncryptedMesh.h"

class RF24NetworkHeader;
class RF24Mesh;
class RF24Network;

namespace RadioEncrypted
{
    class Encryption;

#pragma pack (1)
    struct RadioEncryptedMessage
    {
        MeshAuth header {0, 0, 0};
        EncryptedMessage message {};
        RadioEncryptedMessage() {}
        RadioEncryptedMessage(const MeshAuth & header, const EncryptedMessage & message): header(header), message(message) {}
    };
#pragma pack (0)

    class EncryptedRadio: public IEncryptedMesh
    {
        public:
            EncryptedRadio(uint8_t nodeId, RF24 & radio, Encryption & encryption);
            bool send(const void * data, size_t len, uint8_t messageType, uint16_t toNode = 0, uint8_t retries = 5, uint16_t fallbackNode = 0);
            bool receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint8_t expectFromAddress = 0);
            bool isAvailable();
            uint16_t getNodeId();

#ifdef DEBUG_NETWORK
            void printBytes(void * message, size_t length);
#endif

        private:
            bool sendToNode(const RadioEncryptedMessage & radioMessage, uint16_t toNode, uint8_t retries = 5);
            uint8_t nodeId {0};
            RF24 & radio;
            Encryption & encryption;
    };
}

#endif
