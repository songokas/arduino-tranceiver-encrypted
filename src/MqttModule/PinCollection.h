#ifndef MQTT_MODULE_PIN_COLLECTION_H
#define MQTT_MODULE_PIN_COLLECTION_H

namespace MqttModule
{
    class Pin;

    class PinCollection
    {
        public:
            PinCollection(Pin * pins, uint8_t pinArrLength);

            bool hasAvailablePin(uint8_t pin) const;

            bool set(const Pin & pin);

            bool isReadOnly(uint8_t id) const;

        //@TODO implement iterator
        private:
            Pin * pins {nullptr};
            uint8_t pinArrLength {0};
    };
}
#endif
