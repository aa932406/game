#ifndef _RECHARGEVALUECARD_H_
#define _RECHARGEVALUECARD_H_

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

class RechargeValueCard : public ItemEffect
{
public:
    RechargeValueCard();
    virtual ~RechargeValueCard();

private:
    int32_t m_addon;
};

#endif // _RECHARGEVALUECARD_H_
