#include "fakeit.hpp"
#include "catch.hpp"

#include <RF24Network.h>

#include "MqttModule/MqttMessage.h"
#include "MqttModule/MessageHandlers/IMessageHandler.h"
#include "RadioEncrypted/IEncryptedMesh.h"
#include "MqttModule/MeshMqttClient.h"
#include "MqttModule/SubscriberList.h"

using MqttModule::MqttMessage;
using MqttModule::MeshMqttClient;
using MqttModule::SubscriberList;
using MqttModule::Subscriber;
using MqttModule::MessageHandlers::IMessageHandler;
using RadioEncrypted::IEncryptedMesh;

using namespace fakeit;

TEST_CASE( "MeshMqttClient", "[client]" ) {

    Mock<IEncryptedMesh> meshMock;
    Mock<IMessageHandler> handlerMock;

    When(Method(meshMock, send)).AlwaysReturn(true);
    When(Method(meshMock, receive)).AlwaysDo([](void * data, size_t len, uint8_t messageType, RF24NetworkHeader & header, uint8_t expectFromAddress) {
        MqttMessage message {"topic2", "test"};
        memcpy(data, &message, sizeof(message));
        return true;
    });
    When(Method(meshMock, getNodeId)).AlwaysReturn(4);

    Fake(Method(handlerMock, handle));

    IEncryptedMesh & mesh = meshMock.get();
    IMessageHandler & handler = handlerMock.get();

    IMessageHandler * handlers1[] {nullptr, nullptr};
    IMessageHandler * handlers2[] {nullptr};
    uint16_t nodes1[] {0};
    uint16_t nodes2[] {0};
    Subscriber subs[] { {handlers1, COUNT_OF(handlers1), nodes1, COUNT_OF(nodes1)}, {handlers2, COUNT_OF(handlers2), nodes2, COUNT_OF(nodes2)}};
    SubscriberList subscribers (subs, COUNT_OF(subs));
    MeshMqttClient client(mesh, subscribers); 

    SECTION ("publish full message", "[client]") {
        MqttMessage msg {"test1", "msg1"};
        REQUIRE(client.publish(msg) == true);  
    }

    SECTION ("publish int", "[client]") {
        REQUIRE(client.publish("topicint", (uint16_t)12) == true);  
    }

    SECTION ("subscribe", "[client]") {

        REQUIRE(client.subscribe("topic1", &handler) == true);
        REQUIRE(client.subscribe("topic2", &handler) == true);

        SECTION ("messge arrives", "[client]") {
            When(Method(meshMock, isAvailable)).Return(true, true, true, false, true, false);
            REQUIRE(client.loop() == 3);
            REQUIRE(client.loop() == 1);
            Verify(Method(handlerMock, handle)).Exactly(4);
        
        }
    }

}
