#ifndef RADIO_ENCRYPTED_ENCRYPTED_NETWORK_H
#define RADIO_ENCRYPTED_ENCRYPTED_NETWORK_H

#include <Arduino.h>
#include "IEncryptedMesh.h"

class RF24NetworkHeader;
class RF24Network;

namespace RadioEncrypted
{
    class Encryption;

    class EncryptedNetwork: public IEncryptedMesh
    {
        public:
            EncryptedNetwork(uint16_t nodeId, RF24Network & network, Encryption & encryption);
            bool send(const void * data, size_t len, uint8_t messageType, uint16_t toNode = 0, uint8_t retries = 5, uint16_t forwardThroughNode = 0);
            bool receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint16_t expectFromAddress = 0);
            bool isAvailable();
            uint16_t getNodeId();

#ifdef DEBUG_NETWORK
            void printBytes(void * message, size_t length);
#endif

        private:
            uint16_t nodeId {0};
            RF24Network & network;
            Encryption & encryption;
    };
}

#endif
