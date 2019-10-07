#include <Arduino.h>
#include <Streaming.h>

#include "PinStateHandler.h"
#include "../MqttConfig.h"
#include "../PinCollection.h"
#include "../NodeHelpers.h"
#include "../ValueProviders/ValueProviderFactory.h"

using MqttModule::MessageHandlers::PinStateHandler;
using MqttModule::ValueProviders::ValueProviderFactory;
using MqttModule::Pin;
using MqttModule::PinCollection;

PinStateHandler::PinStateHandler(
    PinCollection & pins,
    ValueProviderFactory & valueProviderFactory
): pins(pins), valueProviderFactory(valueProviderFactory)
{}

/**
 *  # set analog pin 3 to 200
 *  heading/nodes/1234/set/analog/3 200
 *  # sent temperature reading every 10 minutes
 *  heading/nodes/1234/read/temperature/3 600
 */
void PinStateHandler::handle(const char * channel, const char * message)
{
    uint16_t idPos = findPosFromEnd(channel, strlen(channel), '/');
    uint8_t id = atoi(channel + idPos + 1);
    if (!id) {
        Serial << F("Channel id missing: ") << channel << endl;
        return;
    }

    if (pins.hasAvailablePin(id)) {
        Serial << F("No available pins for: ") << channel << endl;
        return;
    }

    uint16_t typePos = findPosFromEnd(channel, idPos - 1, '/');
    if (!typePos) {
        Serial << F("Channel input type missing: ") << channel << endl;
        return;
    }

    uint16_t operationPos = findPosFromEnd(channel, typePos - 1, '/');
    if (!operationPos) {
        Serial << F("Channel operation missing: ") << channel << endl;
        return;
    }

    bool isSetOperation = strncmp("set", channel + operationPos + 1, 3) == 0;
    //bool isAnalog = strncmp("analog", channel + typePos + 1, 6) == 0;
    bool isReadOperation = strncmp("read", channel + operationPos + 1, 4) == 0;

    uint16_t pinType = valueProviderFactory.getMatchingPinType(channel + typePos + 1, 6);
    uint16_t value = atoi(message);

    Pin pin {id, pinType, isSetOperation ? value : 0, false, isReadOperation ? value : DEFAULT_PIN_READ_TIME, 0};
    pins.set(pin);

    if (isSetOperation && !pins.isReadOnly(id)) {
        if (!valueProviderFactory.apply(pin)) {
            Serial << F("Value not applied: ") << channel << endl;
        }
    }
}
