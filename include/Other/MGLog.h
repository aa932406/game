#ifndef _MGLOG_H_
#define _MGLOG_H_

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

class MGLog
{
public:
    void PackageData(Answer::NetPacket *packet) const;

    int64_t Cid;
    int32_t MingGeId;
    int8_t Flag;
    int32_t BagType;
    int32_t Reason;
    int32_t Time;
};

#endif // _MGLOG_H_
