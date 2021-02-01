#ifndef RADIO_ENCRYPTED_IENCRYPTED_MESH_H
#define RADIO_ENCRYPTED_IENCRYPTED_MESH_H

#include <Arduino.h>

class RF24NetworkHeader;

namespace RadioEncrypted
{
    class IEncryptedMesh
    {
        public:

            virtual bool send(const void * data, size_t len, uint8_t messageType, uint16_t toNode = 0, uint8_t retries = 5, uint16_t forwardThroughNode = 0) = 0;
            virtual bool receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint16_t expectFromAddress = 0) = 0;
            virtual bool isAvailable() = 0;
            virtual uint16_t getNodeId() = 0;
    };
}

#endif
