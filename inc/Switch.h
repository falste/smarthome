
#ifndef SWITCH_H_
#define SWITCH_H_

#include <string>
#include <cstdint>

#include "IMqttConnection.h"

class Switch : public IMqttMessageReceiver {
    public:
        Switch(IMqttConnection& mqtt, std::string friendlyName);

        void setOnOff(bool on);
        bool getOnOff(void);

        void onMqttMessage(std::string command, std::string payload) override;

    protected:
        IMqttConnection& mqtt_;
        std::string friendlyName_;

        bool isOn_ = false;
}; // class Switch

#endif // SWITCH_H_
