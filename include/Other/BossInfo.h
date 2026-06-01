#ifndef _BOSSINFO_H_
#define _BOSSINFO_H_

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

class BossInfo
{
public:
    BossInfo();
    BossInfo(const BossInfo& other);

private:
    int32_t m_nBossId;
    std::vector<Position> m_vRevivePos;
};

#endif // _BOSSINFO_H_