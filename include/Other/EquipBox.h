#ifndef _EQUIPBOX_H_
#define _EQUIPBOX_H_

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
#include "Other/ItemEffect.h"

class EquipBox : public ItemEffect
{
public:
    EquipBox();
    virtual ~EquipBox();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    int32_t m_nItemId;
};

#endif // _EQUIPBOX_H_