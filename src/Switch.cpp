
#include "log.h"

#include "Switch.h"

Switch::Switch(IMqttConnection& mqtt, std::string friendlyName) :
    mqtt_(mqtt), friendlyName_(friendlyName){
    mqtt_.registerMessageReceiver(this, friendlyName_);
}

void Switch::setOnOff(bool on) {
    std::string onOff = on ? "\"ON\"" : "\"OFF\"";
    mqtt_.send(friendlyName_, "set", "\"state\"", onOff);
}

bool Switch::getOnOff() {
    return isOn_;
}

void Switch::onMqttMessage(std::string command, std::string payload) {
//    Log(Level::Debug, friendlyName_ + ": " + command + ": " + payload);

    // Update messages don't come with a command
    if (command == "") {
        // TODO: Parse json to update the internal state
        // TODO: Handle update_available somehow
    }
}
