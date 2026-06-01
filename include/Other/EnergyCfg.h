#ifndef _ENERGYCFG_H_
#define _ENERGYCFG_H_

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

class EnergyCfg
{
public:
    EnergyCfg();
    EnergyCfg(const EnergyCfg& a2);

    int32_t MaxEnergy;
    int32_t EnergyTime;
    std::vector<int32_t> EnergyPrice;
    int32_t BuyAddEnergy;
};

#endif // _ENERGYCFG_H_
