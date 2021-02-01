#ifdef __AVR__
#include <avr/wdt.h>
#endif
#include <RF24.h>
#include <RF24Mesh.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "Encryption.h"
#include "EncryptedRadio.h"
#include "Helpers.h"

using RadioEncrypted::Encryption;
using RadioEncrypted::EncryptedRadio;
using RadioEncrypted::EncryptedMessage;
using RadioEncrypted::MeshAuth;
using RadioEncrypted::RadioEncryptedMessage;

EncryptedRadio::EncryptedRadio(uint8_t nodeId, RF24 & radio, Encryption & encryption)
    :nodeId(nodeId), radio(radio), encryption(encryption)
{}

bool EncryptedRadio::send(const void * data, size_t len, uint8_t messageType, uint16_t toNodeId, uint8_t retries, uint16_t fallBackNode)
{
    if  (isAvailable()) {
        error("Packet is available. Read it first");
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

    RadioEncryptedMessage radioMessage { auth, message };

    #ifdef DEBUG_NETWORK
        debug("Send encrypted message"));
        printBytes(&radioMessage, sizeof(radioMessage));
        debug("Auth header"));
        printBytes(&auth, sizeof(auth));
        debug("%d %d %d", auth.fromNode, auth.toNode, auth.messageType);
    #endif

    bool result = sendToNode(radioMessage, toNodeId);
    if (!result && fallBackNode > 0) {
        result = sendToNode(radioMessage, fallBackNode);
    }
    return result;
}

bool EncryptedRadio::sendToNode(const RadioEncryptedMessage & radioMessage, uint16_t toNode, uint8_t retries)
{
    radio.stopListening();
    uint8_t address[6] = {toNode & 0xff, (toNode >> 8), 0, 0, 0, 0};
    radio.openWritingPipe(address);
	while (retries > 0) {
	    resetWatchDog();
	    if (radio.write(&radioMessage, sizeof(radioMessage))) {
            radio.startListening();
            return true;
	    } else {
	        retries--;
	        delay((10 - retries) * 10);
	    }
    }
    radio.startListening();
    return false;
}

bool EncryptedRadio::receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint16_t expectFromAddress)
{
    RadioEncryptedMessage radioMessage;
    radio.read(&radioMessage, sizeof(radioMessage));
    if (radioMessage.header.fromNode < 0) {
        error("Cant verify node id in network");
        return false;
    }

    header.from_node = expectFromAddress > 0 ? expectFromAddress : radioMessage.header.fromNode;
    header.to_node = radioMessage.header.toNode;
    header.type = radioMessage.header.messageType;

    if (radioMessage.header.toNode != nodeId) {
        info("Forward to node %d", radioMessage.header.toNode);
        return sendToNode(radioMessage, radioMessage.header.toNode);
    }

    if (encryption.decrypt(&radioMessage.header, sizeof(radioMessage.header), radioMessage.message, data, len)) {
        return true;
    }
    error("Failed to decrypt message");
    return false;
}

bool EncryptedRadio::isAvailable()
{
    return radio.available();
}

uint16_t EncryptedRadio::getNodeId()
{
    return nodeId;
}

#ifdef DEBUG_NETWORK
void EncryptedRadio::printBytes(void * message, size_t length)
{
    char *byteArray = (char *) message;
    char tmp[16];
    for(size_t i = 0; i < length; ++i){	
        sprintf(tmp, "0x%.2X", byteArray[i]); 
        debug(tmp);
    }
    debug("%d", length);
}

#endif
