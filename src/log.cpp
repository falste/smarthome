
#include <syslog.h>

#include "log.h"

void Log(Level lvl, std::string msg) {
    switch (lvl) {
        case Level::Debug:
            syslog(LOG_DEBUG, "[D] %s", msg.c_str());
            break;
        case Level::Info:
            syslog(LOG_INFO, "[I] %s", msg.c_str());
            break;
        case Level::Warn:
            syslog(LOG_WARNING, "[W] %s", msg.c_str());
            break;
        case Level::Err:
            syslog(LOG_CRIT, "[E] %s", msg.c_str());
            break;
    }
}
