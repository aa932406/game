#ifndef _CDROPITEM_H_
#define _CDROPITEM_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Common/EntityType.h"

// 基础类型定义
typedef long CharId_t;
typedef long EntityId_t;

// 物品结构体
struct ItemData
{
    int32_t nId;
    int8_t nClass;
    int32_t nCount;
    int8_t nCostType;
};

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

class CDropItem : public StaticObj
{
public:
    CDropItem();
    ~CDropItem();

    void reset();
    bool appendInfo(Answer::NetPacket *packet);
    bool checkOwner(CharId_t nCharId, int32_t nTeamId);
    int32_t pick(Player * player);
    bool checkEnd();

private:
    bool m_autoPick;
    bool m_bPicked;
    int32_t m_connid;
    int8_t m_droperType;
    int64_t m_endTick;
    int64_t m_freeTick;
    ItemData m_item;
    int64_t m_mid;
    std::string m_name;
    CharId_t m_owner;
    Map* m_pMap;
    int64_t m_showTick;
    int32_t m_teamId;
};

#endif // _CDROPITEM_H_