#include "log.h"
#include "daemon.h"
#include "Controller.h"
#include "MqttConnection.h"
#include "HttpConnection.h"
#include "PresenceDetector.h"
#include "Timer.h"

#include <stdbool.h>
#include <string>

bool CmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
    if (CmdOptionExists(argv, argv+argc, "--syslog")) {
        SetLogMethod(LogMethod::Syslog);
    } else {
        SetLogMethod(LogMethod::Std);
    }

    setenv("TZ", "/usr/share/zoneinfo/Europe/Berlin", 1);

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

