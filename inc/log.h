#ifndef LOG_H_
#define LOG_H_

#include <string>

enum class LogMethod {
    Std,
    Syslog
};

enum Level {
    Debug=0,
    Info=1,
    Warn=2,
    Err=3
};

void Log(Level lvl, std::string msg);
void SetLogMethod(LogMethod method);

#endif // LOG_H_
