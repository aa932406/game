#ifndef _EQUIPBACKRANKCFG_H_
#define _EQUIPBACKRANKCFG_H_

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

class EquipBackRankCfg
{
public:
    EquipBackRankCfg();
    EquipBackRankCfg(const EquipBackRankCfg& other);

    void PackageData(Answer::NetPacket *packet);

    int64_t Cid;
    std::string nName;
    int32_t Count;
    int32_t Time;
};

#endif // _EQUIPBACKRANKCFG_H_