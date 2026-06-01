#ifndef _LITTLEHELPERCARD_H_
#define _LITTLEHELPERCARD_H_

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

class LittleHelperCard : public ItemEffect
{
public:
    LittleHelperCard();
    virtual ~LittleHelperCard();

private:
    int64_t m_nAddTime;
    int32_t m_nId;
    int32_t m_nLittleHelpId;
};

#endif // _LITTLEHELPERCARD_H_
