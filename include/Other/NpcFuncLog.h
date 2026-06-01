#ifndef _NPCFUNCLOG_H_
#define _NPCFUNCLOG_H_

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

class NpcFuncLog
{
public:
    NpcFuncLog();
    NpcFuncLog(const NpcFuncLog& a2);

    int32_t nCid;
    std::string sName;
    int32_t nFuncId;
    int32_t nMapId;
    int32_t nX;
    int32_t nY;
    int32_t nTime;
};

#endif // _NPCFUNCLOG_H_
