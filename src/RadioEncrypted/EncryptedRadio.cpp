#include <avr/wdt.h>
#include <RF24.h>
#include <RF24Mesh.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <Acorn128.h>

#include "CommonModule/MacroHelper.h"
#include "Encryption.h"
#include "EncryptedRadio.h"

using RadioEncrypted::Encryption;
using RadioEncrypted::EncryptedRadio;

EncryptedRadio::EncryptedRadio(uint8_t nodeId, RF24 & radio, Encryption & encryption)
    :nodeId(nodeId), radio(radio), encryption(encryption)
{}

bool EncryptedRadio::send(const void * data, size_t len, uint8_t messageType, uint16_t toNodeId, uint8_t retries)
{
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

    RadioEncryptedMessage radioMessage { auth, message };

    #ifdef DEBUG_NETWORK
        DPRINTLN(F("Send encrypted message"));
        printBytes(&message, sizeof(message));
        DPRINTLN(F("Auth header"));
        printBytes(&auth, sizeof(auth));
        DPRINTLN(sizeof(message));
    #endif

    radio.stopListening();
    uint8_t address[6] = {toNodeId, 0, 0, 0, 0, 0};
    radio.openWritingPipe(address);
	while (retries > 0) {
	    wdt_reset();
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

bool EncryptedRadio::receive(void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header)
{
    RadioEncryptedMessage radioMessage;
    radio.read(&radioMessage, sizeof(radioMessage));
    if (radioMessage.header.fromNode < 0) {
        DPRINTLN(F("Cant verify node id in network"));
        return false;
    }

    header.from_node = radioMessage.header.fromNode;
    header.to_node = radioMessage.header.toNode;
    header.type = radioMessage.header.messageType;

    if (encryption.decrypt(&radioMessage.header, sizeof(radioMessage.header), radioMessage.message, data, len)) {
        return true;
    }
    DPRINTLN(F("Failed to decrypt message"));
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
        //DPRINT(byte, HEX);
        sprintf(tmp, "0x%.2X", byteArray[i]); 
        DPRINT(tmp);
    }
    DPRINTLN("");
    DPRINTLN(length);
}

#endif
