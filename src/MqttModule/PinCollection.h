#ifndef MQTT_MODULE_PIN_COLLECTION_H
#define MQTT_MODULE_PIN_COLLECTION_H

namespace MqttModule
{
    class Pin;

    class PinCollection
    {
        public:
            bool hasAvailablePin(uint8_t pin) const;

            bool set(const Pin & pin);

            bool isReadOnly(uint8_t id) const;

        protected:
            virtual Pin * getPin(uint8_t id) = 0;
            virtual const Pin * getPin(uint8_t id) const = 0;

    };

    template<uint8_t maxPins>
    class StaticPinCollection: public PinCollection
    {
        public:
            StaticPinCollection(Pin (&pins)[maxPins]): pins(pins)
            {}

        protected:
            Pin * getPin(uint8_t id) { for (auto & pin: pins) { if (pin.id == id) { return &pin; } } return nullptr; }
            const Pin * getPin(uint8_t id) const { for (const auto & pin: pins) { if (pin.id == id) { return &pin; } } return nullptr; }

        private:
            Pin pins[maxPins] {};
    };
}
#endif
