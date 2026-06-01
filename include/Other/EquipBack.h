#ifndef _EQUIPBACK_H_
#define _EQUIPBACK_H_

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

class EquipBack
{
public:
    EquipBack();
    EquipBack(const EquipBack& a2);

    int32_t nId;
    int32_t nType;
    std::list<int32_t> nEquipList;
    int32_t nRecovType;
    int32_t nRecovValues;
    int32_t nBuyBackType;
    int32_t nBuyBackValue;
};

#endif // _EQUIPBACK_H_
