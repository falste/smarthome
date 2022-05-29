
#ifndef MONOLIGHT_H_
#define MONOLIGHT_H_

#include <string>
#include <cstdint>

#include "IMqttConnection.h"

class MonoLight : public IMqttMessageReceiver {
    public:
        MonoLight(IMqttConnection& mqtt, std::string friendlyName);

        void setBrightness(uint8_t percent);
        uint8_t getBrightness(void);

        void onMqttMessage(std::string command, std::string payload) override;

    protected:
        IMqttConnection& mqtt_;
        std::string friendlyName_;

        uint8_t brightness_ = 0;
}; // class MonoLight

#endif // MONOLIGHT_H_
