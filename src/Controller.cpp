
#include <thread>
#include <chrono>

#include "Controller.h"
#include "log.h"

Controller::Controller(IHttpConnection& httpConnection, IMqttConnection& mqttConnection, IPresenceDetector& presenceDetector, ITimer& timer) :
    httpConnection_(httpConnection),
    mqttConnection_(mqttConnection),
    presenceDetector_(presenceDetector),
    timer_(timer),
    kitchenLight_(new MonoLight(mqttConnection, "Tradfri_Light_Kitchen")),
    deskLight_(new RGBLight(mqttConnection, "Mueller_Light_Desk")),
    cornerLight_(new RGBLight(mqttConnection, "Ledvance_Light_Corner")),
    chainLights_(new Switch(mqttConnection, "Tuya_Plug")) {

    httpConnection.registerPostMessageReceiver(this);
    presenceDetector.registerPresenceReceiver(this);
    timer.registerTimeUpdateReceiver(this);

    onPresenceChanged(presenceDetector.getPresence()); // Aquire inital data

    if (!useAutomation_) {
        setScene(0);
    }
}

Controller::~Controller() {

}

void Controller::loop() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Controller::setScene(uint8_t sceneIndex) {
    lastScene_ = sceneIndex;

    switch (sceneIndex) {
        case 0:
            kitchenLight_->setBrightness(100);
            deskLight_->setHSB(40,55,80);
            cornerLight_->setHSB(45,80,100);
            chainLights_->setOnOff(true);
            break;
        case 1:
            kitchenLight_->setBrightness(40);
            deskLight_->setHSB(285,100,80);
            cornerLight_->setHSB(295,100,100);
            chainLights_->setOnOff(true);
            break;
        case 2:
            kitchenLight_->setBrightness(40);
            deskLight_->setHSB(316,100,80);
            cornerLight_->setHSB(316,100,100);
            chainLights_->setOnOff(true);
            break;
        case 3:
            kitchenLight_->setBrightness(40);
            deskLight_->setHSB(344,100,80);
            cornerLight_->setHSB(334,100,100);
            chainLights_->setOnOff(true);
            break;
        case 4:
            kitchenLight_->setBrightness(5);
            deskLight_->setBrightness(0);
            cornerLight_->setBrightness(0);
            chainLights_->setOnOff(true);
            break;
        case 5:
            kitchenLight_->setBrightness(0);
            deskLight_->setBrightness(0);
            cornerLight_->setBrightness(0);
            chainLights_->setOnOff(false);
            break;
        default:
            Log(Level::Err, "Unknown sceneIndex " + std::to_string(sceneIndex));
    }
}

void Controller::onPostMessage(std::string key, std::string value) {
    if (key == std::string("scene")) {
        setScene(std::stoi(value));
    } else if (key == std::string("auto")) {
        if (value == std::string("On")) {
            useAutomation_ = true;
            Log(Level::Debug, "Automation on");

            // Handle all things that should be automated
            onPresenceChanged(presenceDetector_.getPresence());
        } else if (value == std::string("Off")) {
            useAutomation_ = false;
            Log(Level::Debug, "Automation off");
        } else {
            Log(Level::Warn, "Unknown POST value '" + value + "' for key '" + key + "'");
        }
    } else {
        Log(Level::Warn, "Unknown POST key '" + key + "'");
    }
}

void Controller::onPresenceChanged(PresenceState presence) {
    presence_ = presence;

    if (!useAutomation_) {
        return;
    }

    if (presence_ == PresenceState::Away) {
        kitchenLight_->setBrightness(0);
        deskLight_->setBrightness(0);
        cornerLight_->setBrightness(0);
        chainLights_->setOnOff(false);
    } else {
        setScene(lastScene_);
    }
}

void Controller::onFiveMinutes(std::tm time) {
    if (!useAutomation_) {
        return;
    }
    if (presence_ == PresenceState::Away) {
        return;
    }

    if (lastScene_ == 0 && time.tm_hour == 22 && time.tm_min == 0) {
        setScene(3);
    }
}
