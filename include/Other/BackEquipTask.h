#ifndef _BACKEQUIPTASK_H_
#define _BACKEQUIPTASK_H_

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
struct MemChrBag;

class BackEquipTask
{
public:
    BackEquipTask();
    ~BackEquipTask();
    BackEquipTask(const BackEquipTask& other);

private:
    int32_t Index;
    std::vector<int> Equips;
    int32_t PlayerLevelMin;
    int32_t PlayerLevelMax;
    int32_t Star;
    int32_t GetCurr;
    int32_t RandGold;
    std::vector<MemChrBag> Items;
};

#endif // _BACKEQUIPTASK_H_
