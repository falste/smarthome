
#include "RGBLight.h"

RGBLight::RGBLight(IMqttConnection& mqttConnection, std::string friendlyName) : MonoLight(mqttConnection, friendlyName) {

}

RGBLight::~RGBLight() {

}

void RGBLight::setHSB(uint16_t hue, uint8_t sat, uint8_t brightness) {
    setBrightness(brightness);
    std::string payload = std::string("{\"color\":{") +
         "\"h\":" + std::to_string(hue) +
        ",\"s\":" + std::to_string(sat) + "},\"transition\":0}";
//        ",\"b\":" + std::to_string(brightness) + "}}";

    mqtt_.send(friendlyName_, "set", payload);
}

void RGBLight::setRGB(uint8_t r, uint8_t g, uint8_t b) {

}

std::tuple<uint8_t, uint8_t, uint8_t> RGBLight::getRGB() {
    return std::tuple<uint8_t, uint8_t, uint8_t>(1,2,3);
}

std::tuple<uint16_t, uint8_t, uint8_t> RGBLight::getHSB() {
    return std::tuple<uint16_t, uint8_t, uint8_t>(1,2,3);
}
