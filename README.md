# helpers for working with mqtt, nrf, encryption in arduino

## Structure

* src/RadioEncrypted - sending encrypted messages using nrf24l01
* src/MqttModule - sending/receiving mqtt messages
* src/CommonModule - macro helpers

## How to use encrypted mesh

```
# create encrypted mesh to send messages

RF24 radio(CE_PIN, CN_PIN);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

Acorn128 cipher;
EntropyClass entropy;
entropy.initialize();
AvrEntropyAdapter entropyAdapter(entropy);
Encryption encryption (cipher, SHARED_KEY, entropyAdapter);
EncryptedMesh encMesh (mesh, network, encryption);
int data = 1;
uint8_t messageType = 1;
int toNodeId = 3;
encMesh.send(&data, sizeof(data), messageType, toNodeId);

```

## How to use mqtt client

```
// provide a class to handle received messages
class MessageHandler: public IMessageHandler
{
    public:
        void handle(const char * channel, const char * message)
        {
            Serial.println("Received new message:");
            Serial.println(channel);
            Serial.println(message);
        }
};

SubscriberList subscribers;
MeshMqttClient client(encMesh, subscribers);
MessageHandler handler;
client.subscribe("topic", &handler);

while (true) {
    // wait for messages
    client.loop();
    // send message
    client.publish("topic", "message");
    sleep(1000);
}

```

## Examples

https://github.com/songokas/arduino-mqtt-node
