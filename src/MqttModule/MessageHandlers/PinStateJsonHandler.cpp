#include <Arduino.h>
#include <Streaming.h>
#include <ArduinoJson.h>

#ifdef __AVR__
#include <MemoryFree.h>
#endif

#include "PinStateJsonHandler.h"
#include "../MqttConfig.h"
#include "../PinCollection.h"
#include "../NodeHelpers.h"
#include "../ValueProviders/ValueProviderFactory.h"

using MqttModule::ValueProviders::ValueProviderFactory;
using MqttModule::MessageHandlers::PinStateJsonHandler;
using MqttModule::Pin;
using MqttModule::PinCollection;

PinStateJsonHandler::PinStateJsonHandler(
    PinCollection & pins,
    ValueProviderFactory & valueProviderFactory
): pins(pins), valueProviderFactory(valueProviderFactory)
{}

/**
*  listens to json messages sent
*  # set analog pin 3 to 300 and sent data every 5 minutes
*  {"pin": 3, "set": 300, "type": "analog", "readInteval": 300}
*  # set pin 4 to value 1 timeout after 600 seconds
*  {"pin": 4, "set": 1, "timeout": 600}
*/
void PinStateJsonHandler::handle(const char * channel, const char * message)
{
#ifdef __AVR__
    Serial << F("freeMemory ") << freeMemory() << " " << message << endl;
#endif
    DynamicJsonDocument json(MAX_LEN_JSON_MESSAGE);
    DeserializationError error = deserializeJson(json, message);
    if (error) {
        Serial << F("incorrect json format. ") << error.c_str() << endl;
        return;
    }
    if (!json.is<JsonObject>()) {
        Serial << F("incorrect json format. should be: {pin:3, set:3}") << endl;
        return;
    }

    uint8_t id = assignPin(json.as<JsonObject>());
    if (id > 0) {
        Serial << F("Pin updated: ") << id << endl;
    } else {
        Serial << F("Pin update failed") << endl;
    }
}

uint8_t PinStateJsonHandler::assignPin(const JsonObject & json)
{
    if (!json[F("id")].is<uint8_t>()) {
        return 0;
    }

    uint8_t id = json[F("id")].as<uint8_t>();

    if (!pins.hasAvailablePin(id)) {
        return 0;
    }

    const char * type = json[F("type")].is<const char *>() ? json[F("type")].as<const char *>() : nullptr;
    const char * pinType = valueProviderFactory.getMatchingPinType(type, strlen(type));

    Pin pin {
        id,
        pinType,
        json[F("set")].is<uint16_t>() ? json[F("set")].as<uint16_t>() : 0,
        false,
        json[F("readInterval")].is<uint16_t>() ? json[F("readInterval")].as<uint16_t>() : DEFAULT_PIN_READ_TIME
    };

    pins.set(pin);

    if (json[F("set")].is<uint16_t>() && !pins.isReadOnly(id)) {
        if (!valueProviderFactory.apply(pin)) {
            Serial << F("Value not applied: ") << pin.id << endl;
        }
    }
    return pin.id;
}

