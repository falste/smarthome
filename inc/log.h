#ifndef LOG_H_
#define LOG_H_

#include <string>

enum class Level { Debug, Info, Warn, Err };

void Log(Level lvl, std::string msg);

#endif // LOG_H_
