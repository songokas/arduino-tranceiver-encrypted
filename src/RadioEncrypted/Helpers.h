#ifndef RADIO_ENCRYPTED_HELPERS
#define RADIO_ENCRYPTED_HELPERS

namespace RadioEncrypted
{
    inline void resetWatchDog()
    {
        #ifdef __AVR__
            wdt_reset();
        #elif defined(__ESP__)
            ESP.wdtFeed();
        #endif
    }

#ifdef __RF24MESH_H__
    inline bool connectToMesh(RF24Mesh & mesh)
    {
        resetWatchDog();
        if (!mesh.checkConnection() ) {
            resetWatchDog();
            info("Renewing Mesh Address");
            if(!mesh.renewAddress(MESH_TIMEOUT)){
                resetWatchDog();;
                return mesh.begin(RADIO_CHANNEL, RF24_250KBPS, MESH_TIMEOUT);
            } else {
                return true;
            }
        } else {
            resetWatchDog();;
            auto currentAddress = mesh.getAddress(mesh.getNodeID());
            if (!(currentAddress > 0)) {
                info("Renew address: %s", currentAddress);
                resetWatchDog();
                return mesh.renewAddress(MESH_TIMEOUT);
            } else {
                return true;
            }
        }
        return false;
    }
#endif

#ifdef __RF24NETWORK_H__
    inline bool connectToNetwork(RF24Network & network, RF24 & radio, uint16_t useAddress, uint8_t channel)
    {
        debug("Starting radio...");
        radio.setChannel(channel);
        radio.setDataRate(RF24_250KBPS);
        radio.setPALevel(RF24_PA_MAX);
        uint8_t retryRadio = 10;
        while (retryRadio-- > 0) {
            resetWatchDog();
            if (!radio.begin()) {
                error("Failed to initialize radio");
                delay(100);
                continue;
            }
            if (radio.isChipConnected()) {
                debug("Initialized.");
                break;
            }
        }
        network.begin(channel, useAddress);
        if (!radio.isChipConnected()) {
            return false;
        }
        return true;
    }
#endif
#ifdef __RF24_H__
    inline bool connectToRadio(RF24 & radio, uint8_t useAddress, uint8_t forwardAddress, uint8_t channel, uint8_t retry = 10)
    {
        debug("Starting radio...");

        while (retry-- > 0) {
            resetWatchDog();
            if (!radio.begin()) {
                error("Failed to initialize radio");
                delay(100);
                continue;
            }
            if (radio.isChipConnected()) {
                debug("Initialized.");
                break;
            }
        }
        if (!radio.isChipConnected()) {
            return false;
        }
        const uint8_t slave[] = {useAddress, 0, 0, 0, 0, 0};
        const uint8_t forwardTo[] = {forwardAddress, 0, 0, 0, 0, 0};

        radio.openReadingPipe(1, slave);
        radio.openReadingPipe(2, forwardTo);
        radio.setChannel(channel);
        radio.setDataRate(RF24_250KBPS);
        radio.setPALevel(RF24_PA_MAX);
        radio.startListening();
        return true;
    }
#endif
#ifdef PubSubClient_h
    inline bool sendLiveData(PubSubClient & client)
    {
        char topic[sizeof(CHANNEL_KEEP_ALIVE)] {0};
        snprintf_P(topic, COUNT_OF(topic), CHANNEL_KEEP_ALIVE);
        char liveMsg[16] {0};
        sprintf(liveMsg, "%d", millis());
        if (!client.publish(topic, liveMsg)) {
            error("Failed to publish keep alive");
            return false;
        }	
        return true;
    }

    inline bool connectToMqtt(PubSubClient & client, const char * clientName, const char * subscribeTopic, uint8_t retry = 6)
    {
        if (client.connected()) {
            return true;
        }
        info("Attempting MQTT connection...");
        uint16_t mqttAttempts = 0;
        while (!client.connect(clientName) && mqttAttempts < retry) {
            mqttAttempts++;
            info("retry...");
            delay(60 * mqttAttempts);
            resetWatchDog();
        }
        if (mqttAttempts >= retry) {
            error("Mqtt connection failed, rc=%s try again in 5 seconds", client.state());
            return false;
        } 

        info("connected.");
        if (subscribeTopic != nullptr) {
            if (client.subscribe(subscribeTopic)) {
                info("Subscribed to: %s", subscribeTopic);
            } else {
                warning("Failed to subscribe to: %s", subscribeTopic);
            }
        }
        resetWatchDog();
        return true;
    }
#endif
#ifdef WIFICLIENTMULTI_H_

    inline bool connectToWifi(ESP8266WiFiMulti & wifi, uint8_t retries = 10)
    {
        if (wifi.run() == WL_CONNECTED) {
            return true;
        }
        info("Wait for WiFi... ");
        uint16_t wifiAttempts = 0;
        while (wifi.run() != WL_CONNECTED) {
            wifiAttempts++;
            Serial.print(".");
            delay(50 * wifiAttempts);
            resetWatchDog();
            if (wifiAttempts >= retries) {
                return false;
            }
        }
        Serial.println();
        info("Wifi connected.");// IP address: %s", WiFi.localIP().toString());
        return true;
    }
#endif
}

#endif
