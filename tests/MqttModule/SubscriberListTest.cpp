#include "fakeit.hpp"
#include "catch.hpp"

#include <string.h>

#include "MqttModule/MqttMessage.h"
#include "MqttModule/MessageHandlers/IMessageHandler.h"
#include "MqttModule/SubscriberList.h"

using MqttModule::MqttMessage;
using MqttModule::SubscriberList;
using MqttModule::Subscriber;
using MqttModule::MessageHandlers::IMessageHandler;

using namespace fakeit;

TEST_CASE( "subscribtion add", "[subscriber]" ) {

    IMessageHandler * handlers1[] {nullptr, nullptr};
    IMessageHandler * handlers2[] {nullptr, nullptr};
    uint16_t nodes1[] {0, 0};
    uint16_t nodes2[] {0, 0};
    Subscriber subs[] { {handlers1, COUNT_OF(handlers1), nodes1, COUNT_OF(nodes1)}, {handlers2, COUNT_OF(handlers2), nodes2, COUNT_OF(nodes2)}};
    SubscriberList list (subs, COUNT_OF(subs));

    SECTION( "add subscribers" ) {
        Mock<IMessageHandler> handler1;
        Mock<IMessageHandler> handler2;

        Fake(Method(handler1,handle));
        Fake(Method(handler2,handle));

        IMessageHandler &h1 = handler1.get();
        IMessageHandler &h2 = handler2.get();
        REQUIRE(list.add("topic1", &h1, 12) == true);
        REQUIRE(list.countHandlers("topic1") == 1);
        REQUIRE(list.add("topic1", &h1, 13) == true);
        REQUIRE(list.countHandlers("topic1") == 1);
        REQUIRE(list.add("topic1", &h1, 12) == false);
        REQUIRE(list.countHandlers("topic1") == 1);
        REQUIRE(list.add("topic1", &h2, 13) == true);
        REQUIRE(list.countHandlers("topic1") == 2);
        REQUIRE(list.add("topic2", &h2, 13) == true);
        REQUIRE(list.countHandlers("topic1") == 2);
        REQUIRE(list.countHandlers("topic2") == 1);

        SECTION( "call subscribers" ) {

            MqttMessage m1 {"hey", "you"};
            REQUIRE(list.call(m1) == 0);
            MqttMessage m2 {"topic1", "m1"};
            REQUIRE(list.call(m2) == 2);
            MqttMessage m3 {"topic2", "m2"};
            REQUIRE(list.call(m3) == 1);

            Verify(Method(handler1, handle)).Exactly(1);
            Verify(Method(handler2, handle)).Exactly(2);

        }
    }
}
