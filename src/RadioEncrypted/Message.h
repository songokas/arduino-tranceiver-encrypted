#ifndef RADIO_ENCRYPTED_RADIO_MESSAGE_H
#define RADIO_ENCRYPTED_RADIO_MESSAGE_H

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

#pragma pack (1)
    struct EncryptedMessage
    {
        uint8_t nonce[ENCRYPTION_VECTOR_LENGTH] {0};
        uint8_t cipherText[ENCRYPTION_CIPHER_LENGTH] {0};
        uint8_t tag[ENCRYPTION_TAG_LENGTH] {0};
    };
#pragma pack (0)

#pragma pack (1)
    struct RadioEncryptedMessage
    {
        MeshAuth header {0, 0, 0};
        EncryptedMessage message {};
        RadioEncryptedMessage() {}
        RadioEncryptedMessage(const MeshAuth & header, const EncryptedMessage & message): header(header), message(message) {}
    };
#pragma pack (0)
}

#endif