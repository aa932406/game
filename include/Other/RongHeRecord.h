#ifndef _RONGHERECORD_H_
#define _RONGHERECORD_H_

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

class RongHeRecord
{
public:
    RongHeRecord(const RongHeRecord\& a2);

    int32_t nCid;
    std::string strName;
    int32_t nCostId;
    int32_t nGiveId;
    int32_t nSuccess;
    int32_t nTime;
};

#endif // _RONGHERECORD_H_
