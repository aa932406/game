#ifndef _ANSWER_H_
#define _ANSWER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Common/CommonTypes.h"
#include "Other/RwLock.h"
#include "Network/NetPacket.h"

class Player;
class Map;
class Unit;
class Npc;
class CActivity;

namespace Answer {

// 日志级别
enum class LogLevel : uint8_t {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3
};

class Logger {
public:
    static void print(const char* format, ...);
    static void print(int level, const char* format, ...);
    static void info(const char* format, ...);
    static void error(const char* format, ...);
};

class MySqlDBGuard;
class MySqlQuery;
class DayTime;
class Random;

class DBPool {
public:
    MySqlDBGuard* GetDBGuard();
};

constexpr int LOG_LEVEL_ERROR = 3;
constexpr int LOG_LEVEL_DEBUG = 0;
constexpr int LOG_LEVEL_INFO = 1;
constexpr int LOG_LEVEL_WARN = 2;

int setkeepalive(int fd, int idleseconds);
int setnodelay(int fd);
int setnonblock(int fd);
int setreuseaddr(int fd);
int setreuseport(int fd);

} // namespace Answer

#endif // _ANSWER_H_
