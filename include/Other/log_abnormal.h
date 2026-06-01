#ifndef _LOG_ABNORMAL_H_
#define _LOG_ABNORMAL_H_

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

class log_abnormal
{
public:
    log_abnormal();
    log_abnormal(const log_abnormal& other);

    void PackageData(Answer::NetPacket *packet);

    int64_t nCid;
    std::string sName;
    int32_t nType;
    int32_t nValue;
    int32_t nOpway;
};

#endif // _LOG_ABNORMAL_H_