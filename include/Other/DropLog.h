#ifndef _DROPLOG_H_
#define _DROPLOG_H_

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

class DropLog
{
public:
    DropLog();
    ~DropLog();

    int64_t     nCid;
    int32_t     nEquipId;
    int64_t     nSrcId;
    int32_t     nMapId;
    int64_t     nKiller;
    std::string sKillerName;
    std::string sName;
};

#endif // _DROPLOG_H_
