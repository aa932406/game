#ifndef _CURRENCYRES_H_
#define _CURRENCYRES_H_

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

class CurrencyRes : public ItemEffect
{
public:
    CurrencyRes();
    virtual ~CurrencyRes();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    int32_t m_id;
    int32_t m_CurrecyType;
    int32_t m_CurrecyCount;
};

#endif // _CURRENCYRES_H_