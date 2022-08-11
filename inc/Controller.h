
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <string>
#include <ctime>
#include <memory>

#include "MonoLight.h"
#include "RGBLight.h"
#include "Switch.h"
#include "IHttpConnection.h"
#include "IMqttConnection.h"
#include "IPresenceDetector.h"
#include "ITimer.h"

class Controller : public IPostMessageReceiver, public IPresenceReceiver, public ITimeUpdateReceiver {
    public: // Singleton pattern
        static Controller& getInstance(IHttpConnection& httpConnection, IMqttConnection& mqttConnection, IPresenceDetector& presenceDetector, ITimer& timer) {
            static Controller instance(httpConnection, mqttConnection, presenceDetector, timer);
            return instance;
        }

        Controller(Controller const&) = delete;
        void operator=(Controller const&) = delete;

    private:
        Controller(IHttpConnection& httpConnection, IMqttConnection& mqttConnection, IPresenceDetector& presenceDetector, ITimer& timer);
        ~Controller();

    public:
        void loop(void);

    private:
        void onPostMessage(std::string key, std::string value);
        void onPresenceChanged(PresenceState presence);
        void onFiveMinutes(std::tm time);

        void setScene(uint8_t sceneIndex);

    private:
        IHttpConnection& httpConnection_;
        IMqttConnection& mqttConnection_;
        IPresenceDetector& presenceDetector_;
        ITimer& timer_;

        std::unique_ptr<MonoLight> kitchenLight_;
        std::unique_ptr<RGBLight> deskLight_;
        std::unique_ptr<RGBLight> cornerLight_;
        std::unique_ptr<Switch> chainLights_;

        uint8_t lastScene_ = 0;
        bool useAutomation_ = true;
        PresenceState presence_ = PresenceState::Present;

}; // class Controller

#endif // CONTROLLER_H_

