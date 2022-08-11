
#include "base/log.h"

#include "MonoLight.h"

constexpr uint8_t maxBrightness = 254;

MonoLight::MonoLight(IMqttConnection& mqtt, std::string friendlyName) :
    mqtt_(mqtt), friendlyName_(friendlyName){
    mqtt_.registerMessageReceiver(this, friendlyName_);
}

void MonoLight::setBrightness(uint8_t percent) {
    mqtt_.send(friendlyName_, "set", "{\"brightness\":" + std::to_string(percent * maxBrightness / 100) + ",\"transition\":2}");
}

/*
void MonoLight::setOnOff(bool on) {
    std::string onOff = on ? "ON" : "OFF";
    mqtt_.send(friendlyName_, "set", "\"state\"", onOff);
}

void MonoLight::toggle() {
    mqtt_.send(friendlyName_, "set", "\"state\"", "\"TOGGLE\"");
}
*/

uint8_t MonoLight::getBrightness() {
    return brightness_;
}

/*
bool MonoLight::getOnOff() {
    return brightness_ != 0;
}
*/

void MonoLight::onMqttMessage(std::string command, std::string payload) {
//    Log(Level::Debug, friendlyName_ + ": " + command + ": " + payload);

    // Update messages don't come with a command
    if (command == "") {
        // TODO: Parse json to update the internal state
        // TODO: Handle update_available somehow
    }
}
