
#ifndef RGBLIGHT_H_
#define RGBLIGHT_H_

#include <tuple>

#include "IMqttConnection.h"
#include "MonoLight.h"

class RGBLight : public MonoLight {
    public:
        RGBLight(IMqttConnection& mqttConnection, std::string friendlyName);
        ~RGBLight();

        void setHSB(uint16_t hue, uint8_t sat, uint8_t brightness);
        void setRGB(uint8_t r, uint8_t g, uint8_t b);
        std::tuple<uint8_t, uint8_t, uint8_t> getRGB();
        std::tuple<uint16_t, uint8_t, uint8_t> getHSB();
}; // class RGBLight

#endif // RGBLIGHT_H_
