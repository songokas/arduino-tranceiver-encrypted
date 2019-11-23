#ifndef RADIO_ENCRYPTED_IENCRYPTED_MESH_H
#define RADIO_ENCRYPTED_IENCRYPTED_MESH_H

#include <Arduino.h>

class RF24NetworkHeader;

namespace RadioEncrypted
{


#pragma pack (1)
    struct MeshAuth
    {
        uint16_t fromNode {0};
        uint16_t toNode {0};
        uint8_t messageType {0};

        //c++11
        MeshAuth(uint16_t fromNode, uint16_t toNode, uint8_t messageType)
            :fromNode(fromNode), toNode(toNode), messageType(messageType)
        {}
    };
#pragma pack(0)

    class IEncryptedMesh
    {
        public:
            virtual bool send(const void * data, size_t len, uint8_t messageType, uint16_t toNode = 0, uint8_t retries = 5) = 0;
            virtual bool receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint8_t expectFromAddress = 0) = 0;
            virtual bool isAvailable() = 0;
            virtual uint16_t getNodeId() = 0;
    };
}

#endif
