
#include <syslog.h>
#include <iostream>

#include "log.h"

LogMethod logMethod = LogMethod::Std;
const std::string levelPrefixes[4] = {
    "[D] ",
    "[I] ",
    "[W] ",
    "[E] "
};
constexpr int prios[] = {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_CRIT
};

void Log(Level lvl, std::string msg) {
    msg = levelPrefixes[lvl] + msg;

    if (logMethod == LogMethod::Syslog) {
        syslog(prios[lvl], "%s", msg.c_str());
    } else {
        if (lvl == Level::Debug || lvl == Level::Info) {
            std::cout << msg << "\n";
        } else {
            std::cerr << msg << "\n";
        }
    }
}

void SetLogMethod(LogMethod method) {
    logMethod = method;
    std::string sMethod;
    sMethod = method == LogMethod::Std ? "Std" : "Syslog";
    Log(Level::Debug, "Logging method set to " + sMethod);
}
