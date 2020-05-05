#ifdef __AVR__
#include <avr/wdt.h>
#endif
#include <RF24.h>
#include <RF24Network.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "Encryption.h"
#include "EncryptedNetwork.h"
#include "Helpers.h"

using RadioEncrypted::Encryption;
using RadioEncrypted::EncryptedNetwork;
using RadioEncrypted::EncryptedMessage;
using RadioEncrypted::MeshAuth;

EncryptedNetwork::EncryptedNetwork(uint16_t nodeId, RF24Network & network, Encryption & encryption)
    :nodeId(nodeId), network(network), encryption(encryption)
{}

bool EncryptedNetwork::send(const void * data, size_t len, uint8_t messageType, uint16_t toNodeId, uint8_t retries, uint16_t forwardThroughNode)
{
    network.update();
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
    if (!encryption.encrypt(data, len, &auth, sizeof(auth), message)) {
        error("Failed to encrypt data");
        return false;
    }

    RF24NetworkHeader header(toNodeId, (char)messageType);

    #ifdef DEBUG_NETWORK
        debug("Send encrypted message");
        printBytes(&message, sizeof(message));
        debug("Auth header");
        printBytes(&auth, sizeof(auth));
    #endif

	while (retries > 0) {
	    resetWatchDog();
	    if (network.write(header, &message, sizeof(message))) {
            return true;
	    } else {
	        retries--;
	        delay((10 - retries) * 10);
	    }
    }
    return false;
}

bool EncryptedNetwork::receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint16_t expectFromAddress)
{
    network.peek(header);

    if (header.type != messageType && messageType != 0) {
        return false;
    }

    EncryptedMessage message;
    uint16_t bytesRead = network.read(header, &message, sizeof(message));
    if (!(bytesRead > 0)) {
        error("Mesh received 0 bytes read");
        return false;
    }

    int16_t nodeId = expectFromAddress ? expectFromAddress : header.from_node;
    if (nodeId < 0) {
        error("Can not verify node id");
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

bool EncryptedNetwork::isAvailable()
{
    return network.available();
}

uint16_t EncryptedNetwork::getNodeId()
{
    return nodeId;
}

#ifdef DEBUG_NETWORK
void EncryptedNetwork::printBytes(void * message, size_t length)
{
    char *byteArray = (char *) message;
    char tmp[16];
    for(size_t i = 0; i < length; ++i){	
        //DPRINT(byte, HEX);
        sprintf(tmp, "0x%.2X", byteArray[i]); 
        debug("%s", tmp);
    }
    debug("%d", length);
}

#endif
