#ifndef RADIO_ENCRYPTED_ENCRYPTED_MESH_H
#define RADIO_ENCRYPTED_ENCRYPTED_MESH_H

#include <Arduino.h>
#include "IEncryptedMesh.h"

class RF24NetworkHeader;
class RF24Mesh;
class RF24Network;

namespace RadioEncrypted
{
    class Encryption;

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

    class EncryptedMesh: public IEncryptedMesh
    {
        public:
            EncryptedMesh(RF24Mesh & mesh, RF24Network & network, Encryption & encryption);
            bool send(const void * data, size_t len, uint8_t messageType, uint16_t toNode = 0, uint8_t retries = 5);
            bool receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header);
            bool isAvailable();
            uint16_t getNodeId();

#ifdef DEBUG_NETWORK
            void printBytes(void * message, size_t length);
#endif

        private:
            RF24Mesh & mesh;
            RF24Network & network;
            Encryption & encryption;
    };
}

#endif
