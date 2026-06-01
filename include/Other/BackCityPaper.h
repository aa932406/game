#ifndef _BACKCITYPAPER_H_
#define _BACKCITYPAPER_H_

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

class BackCityPaper : public ItemEffect
{
public:
    BackCityPaper(bool bXinMo);
    virtual ~BackCityPaper();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    bool m_BXinMo;
    int32_t m_itemid;
};

#endif // _BACKCITYPAPER_H_
