#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "Encryption.h"
#include "EncryptedMesh.h"

using RadioEncrypted::Encryption;
using RadioEncrypted::EncryptedMesh;

EncryptedMesh::EncryptedMesh(RF24Mesh & mesh, RF24Network & network, Encryption & encryption)
    :mesh(mesh), network(network), encryption(encryption)
{}

bool EncryptedMesh::send(const void * data, size_t len, uint8_t messageType, uint16_t toNodeId, uint8_t retries)
{
    mesh.update();
    if  (isAvailable()) {
        DPRINTLN(F("Packet is available. Read it first"));
        return false;
    }
    EncryptedMessage message;
    if (!encryption.randomBytes(message.nonce, COUNT_OF(message.nonce))) {
        DPRINTLN(F("Failed to create random bytes"));
        return false;
    }

    MeshAuth auth {getNodeId(), toNodeId, messageType};
    encryption.encrypt(data, len, &auth, sizeof(auth), message);

    #ifdef DEBUG_NETWORK
        DPRINTLN(F("Send encrypted message"));
        printBytes(&message, sizeof(message));
        DPRINTLN(F("Auth header"));
        printBytes(&auth, sizeof(auth));
        DPRINTLN(sizeof(message));
    #endif

	while (retries > 0) {
	    if (mesh.write(&message, messageType, sizeof(message), toNodeId)) {
            return true;
	    } else {
	        retries--;
	        delay((10 - retries) * 10);
	    }
    }
    return false;
}

bool EncryptedMesh::receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header)
{
    network.peek(header);

    if (header.type == messageType || messageType == 0) {
        EncryptedMessage message;
        uint16_t bytesRead = network.read(header, &message, sizeof(message));
        if (!(bytesRead > 0)) {
            DPRINTLN(F("Mesh received 0 bytes read"));
            return false;
        }

        MeshAuth auth {mesh.getNodeID(header.from_node), getNodeId(), header.type};

#ifdef DEBUG_NETWORK
        DPRINTLN(F("Received encrypted message"));
        printBytes(&message, sizeof(message));
        DPRINTLN(F("Auth header"));
        printBytes(&auth, sizeof(auth));
        DPRINTLN(bytesRead);
        DPRINTLN(auth.fromNode);
        DPRINTLN(header.toString());
#endif

        if (!encryption.decrypt(&auth, sizeof(auth), message, data, len)) {
            DPRINTLN(F("Failed to decrypt message"));
            return false;
        }
        return true;
    }
    return false;
}

bool EncryptedMesh::isAvailable()
{
    return network.available();
}

uint16_t EncryptedMesh::getNodeId()
{
    return mesh.getNodeID();
}

#ifdef DEBUG_NETWORK
void EncryptedMesh::printBytes(void * message, size_t length)
{
    char *byteArray = (char *) message;
    char tmp[16];
    for(size_t i = 0; i < length; ++i){	
        //DPRINT(byte, HEX);
        sprintf(tmp, "0x%.2X", byteArray[i]); 
        DPRINT(tmp);
    }
    DPRINTLN("");
    DPRINTLN(length);
}

#endif
