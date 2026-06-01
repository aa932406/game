#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

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

class ItemData
{
public:
    bool IsEmpty() const;

    int32_t m_nId;
    int32_t m_nClass;
    int32_t m_nCount;
};

#endif // _ITEMDATA_H_
