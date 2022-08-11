#include "daemon.h"
#include "Controller.h"
#include "MqttConnection.h"
#include "HttpConnection.h"
#include "PresenceDetector.h"
#include "Timer.h"

int main(void) {
    Daemon& daemon = Daemon::getInstance();

    IHttpConnection& httpConnection = HttpConnection::getInstance();
    IMqttConnection& mqttConnection = MqttConnection::getInstance();
    IPresenceDetector& presenceDetector = PresenceDetector::getInstance();
    ITimer& timer = Timer::getInstance();
    Controller& controller = Controller::getInstance(httpConnection, mqttConnection, presenceDetector, timer);

    while (daemon.isRunning()) {
        controller.loop();
    }
}

