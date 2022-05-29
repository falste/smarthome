
#include <chrono>

#include "log.h"
#include "PresenceDetector.h"

using namespace std::literals::chrono_literals;

constexpr std::chrono::duration delay = 5s;
constexpr uint8_t numFailedPings = 5;

PresenceDetector::PresenceDetector() {
    hThread_ = std::thread(&PresenceDetector::detect, this);
}


PresenceDetector::~PresenceDetector() {

}

void PresenceDetector::detect() {
    while (true) {
        bool isPhoneReachable = false;
        bool isRouterReachable = false;

        for (uint8_t i = 0; i < numFailedPings; i++) {
            // Check phone connection
            if (system("ping -c1 -s0 192.168.178.4  > /dev/null 2>&1") == 0) {
                isPhoneReachable = true;
                break;
            }
        }

        for (uint8_t i = 0; i < numFailedPings; i++) {
            // Check router connection
            if (system("ping -c1 -s0 192.168.178.1  > /dev/null 2>&1") == 0) {
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
