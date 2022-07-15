
#include <chrono>
#include <string>

#include "config.h"
#include "log.h"
#include "PresenceDetector.h"

using namespace std::literals::chrono_literals;
constexpr std::chrono::duration delay = 5s;

PresenceDetector::PresenceDetector() {
    hThread_ = std::thread(&PresenceDetector::detect, this);
}


PresenceDetector::~PresenceDetector() {

}

void PresenceDetector::detect() {
    while (true) {
        bool isPhoneReachable = false;
        bool isRouterReachable = false;

        for (uint8_t i = 0; i < cfg::maxFailedPings; i++) {
            // Check phone connection
            std::string cmd = "ping -c1 -s0 " + cfg::phoneIp + " > /dev/null 2>&1";
            if (system(cmd.c_str()) == 0) {
                isPhoneReachable = true;
                break;
            }
        }

        for (uint8_t i = 0; i < cfg::maxFailedPings; i++) {
            // Check router connection
            std::string cmd = "ping -c1 -s0 " + cfg::routerIp + " > /dev/null 2>&1";
            if (system(cmd.c_str()) == 0) {
                isRouterReachable = true;
                break;
            }
        }

        PresenceState newPresence_;
        if (isPhoneReachable) {
            newPresence_ = PresenceState::Present;
        } else if (isRouterReachable) {
            newPresence_ = PresenceState::Away;
        } else {
            newPresence_ = PresenceState::Unknown;
        }


        if (presence_ != newPresence_){
            presence_ = newPresence_;

            switch (presence_) {
                case PresenceState::Present:
                    Log(Level::Debug, "Presence changed to 'Present'");
                    break;
                case PresenceState::Away:
                    Log(Level::Debug, "Presence changed to 'Away'");
                    break;
                case PresenceState::Unknown:
                    Log(Level::Debug, "Presence changed to 'Unknown'");
                    break;
                default:
                    Log(Level::Err, "Presence changed to invalid value");
                    
            }
            
            for (IPresenceReceiver* recv : recv_) {
                recv->onPresenceChanged(presence_);
            }
        }

        std::this_thread::sleep_for(delay);
    }
}

PresenceState PresenceDetector::getPresence() {
    return presence_;
}

void PresenceDetector::registerPresenceReceiver(IPresenceReceiver* recv) {
    if (recv == nullptr) {
        Log(Level::Err, "Trying to register nullptr");
        return;
    }

    recv_.push_back(recv);
}
