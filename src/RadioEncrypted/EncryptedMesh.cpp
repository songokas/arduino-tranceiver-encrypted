#ifdef __AVR__
#include <avr/wdt.h>
#endif
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "Encryption.h"
#include "EncryptedMesh.h"
#include "Helpers.h"

using RadioEncrypted::Encryption;
using RadioEncrypted::EncryptedMesh;
using RadioEncrypted::EncryptedMessage;
using RadioEncrypted::MeshAuth;

EncryptedMesh::EncryptedMesh(RF24Mesh & mesh, RF24Network & network, Encryption & encryption)
    :mesh(mesh), network(network), encryption(encryption)
{}

bool EncryptedMesh::send(const void * data, size_t len, uint8_t messageType, uint16_t toNodeId, uint8_t retries, uint16_t forwardThroughNode)
{
    mesh.update();
    if  (isAvailable()) {
        warning("Packet is available. Read it first");
        return false;
    }
    EncryptedMessage message;
    if (!encryption.randomBytes(message.nonce, COUNT_OF(message.nonce))) {
        error("Failed to create random bytes");
        return false;
    }

    MeshAuth auth {getNodeId(), toNodeId, messageType};
    encryption.encrypt(data, len, &auth, sizeof(auth), message);

    #ifdef DEBUG_NETWORK
        debug("Send encrypted message");
        printBytes(&message, sizeof(message));
        debug("Auth header");
        printBytes(&auth, sizeof(auth));
    #endif

	while (retries > 0) {
	    resetWatchDog();
	    if (mesh.write(&message, messageType, sizeof(message), toNodeId)) {
            return true;
	    } else {
	        retries--;
	        delay((10 - retries) * 10);
	    }
    }
    return false;
}

bool EncryptedMesh::receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint16_t expectFromAddress)
{
    network.peek(header);

    if (header.type == messageType || messageType == 0) {
        EncryptedMessage message;
        uint16_t bytesRead = network.read(header, &message, sizeof(message));
        if (!(bytesRead > 0)) {
            error("Mesh received 0 bytes read");
            return false;
        }

        int16_t nodeId = expectFromAddress ? expectFromAddress : mesh.getNodeID(header.from_node);
        if (nodeId < 0) {
            error("Can not verify node id in network");
            return false;
        }

        MeshAuth auth {(uint16_t)nodeId, getNodeId(), header.type};

#ifdef DEBUG_NETWORK
        debug("Received encrypted message");
        printBytes(&message, sizeof(message));
        debug("Bytes read %d", bytesRead);
        debug("Auth header");
        printBytes(&auth, sizeof(auth));
        debug("%d %d %d %s", auth.fromNode, auth.toNode, auth.messageType, header.toString());
#endif

        if (!encryption.decrypt(&auth, sizeof(auth), message, data, len)) {
            error("Failed to decrypt message");
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
        sprintf(tmp, "0x%.2X", byteArray[i]); 
        debug("%s", tmp);
    }
    debug("%d", length);
}

#endif
