#ifndef _WUHUNITEM_H_
#define _WUHUNITEM_H_

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

struct AddAttribute;

class WuHunItem
{
public:
    WuHunItem(const WuHunItem& a2);

    int32_t nId;
    int32_t nLevel;
    int32_t nType;
    int32_t nQuality;
    int32_t nNeedQuality;
    std::list<AddAttribute> lAttrList;
    int32_t nTalentId;
};

#endif // _WUHUNITEM_H_