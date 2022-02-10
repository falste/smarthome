#include <iostream>
#include <libconfig.h++>

#include "daemon.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string>

#include "Controller.h"
#include "base/MqttConnection.h"
#include "base/HttpConnection.h"
#include "MonoLight.h"
#include "RGBLight.h"
#include "PresenceDetector.h"
#include "Timer.h"

#include <syslog.h>

int main(void) {
    // TODO: Check if I need the daemons reload function

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

