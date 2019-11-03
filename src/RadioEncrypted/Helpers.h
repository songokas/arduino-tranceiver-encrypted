#ifndef RADIO_ENCRYPTED_HELPERS
#define RADIO_ENCRYPTED_HELPERS

#include <RF24Mesh.h>
#include <Streaming.h>

namespace RadioEncrypted
{

    inline bool reconnect(RF24Mesh & mesh)
    {
        wdt_reset();
        if (!mesh.checkConnection() ) {
            wdt_reset();
            Serial << F("Renewing Mesh Address") << endl;
            if(!mesh.renewAddress(MESH_TIMEOUT)){
                wdt_reset();
                return mesh.begin(RADIO_CHANNEL, RF24_250KBPS, MESH_TIMEOUT);
            } else {
                return true;
            }
        } else {
            wdt_reset();
            auto currentAddress = mesh.getAddress(mesh.getNodeID());
            if (!(currentAddress > 0)) {
                Serial << F("Renew address: ") << currentAddress << endl;
                wdt_reset();
                return mesh.renewAddress(MESH_TIMEOUT);
            } else {
                return true;
            }
        }
        return false;
    }

}

#endif
