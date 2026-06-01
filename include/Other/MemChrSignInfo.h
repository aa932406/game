#ifndef _MEMCHRSIGNINFO_H_
#define _MEMCHRSIGNINFO_H_

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
class Position;class MemChrSignInfo
{
public:
    MemChrSignInfo();

    void CleanUp();

    int32_t sign_record;
    int32_t refresh_time;
    std::vector<int8_t> sign_reward;
};

#endif // _MEMCHRSIGNINFO_H_