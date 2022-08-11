#include "daemon.h"
#include "log.h"

Daemon::Daemon() {
    m_isRunning = true;
    m_reload = false;
    signal(SIGINT, Daemon::signalHandler);
    signal(SIGTERM, Daemon::signalHandler);
    signal(SIGHUP, Daemon::signalHandler);
}

void Daemon::setReloadFunction(std::function<void()> func) {
    m_reloadFunc = func;
}

bool Daemon::isRunning() {
    if (m_reload) {
        m_reload = false;
        m_reloadFunc();
    }
    return m_isRunning;
}

void Daemon::signalHandler(int signal) {
    Log(Level::Info, "Interrupt signal number [" + std::to_string(signal) + "] recieved.");
    switch (signal) {
        case SIGINT:
        case SIGTERM: {
            Daemon::getInstance().m_isRunning = false;
            break;
        }
        case SIGHUP: {
            Daemon::getInstance().m_reload = true;
            break;
        }
    }
}
