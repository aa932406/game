#ifndef _PLATFORM_LOG_BASE_H_
#define _PLATFORM_LOG_BASE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class platform_log_base
{
public:
    platform_log_base();
    ~platform_log_base();
    virtual void CleanUp();

    int32_t log_time;
    int32_t log_Sid;

    virtual void PackageData(Answer::NetPacket *packet);
    virtual void UnPackageData(Answer::NetPacket *inPacket);
};

#endif // _PLATFORM_LOG_BASE_H_
