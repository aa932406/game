#ifndef _CDROPITEM_H_
#define _CDROPITEM_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Common/CommonTypes.h"
#include "Common/EntityType.h"
#include "Other/StaticObj.h"

// 前置声明
class Player;
class Map;

namespace Answer { class NetPacket; }

// 掉落物品数据结构
struct DropItemData
{
    int32_t nId;           // 物品ID
    int8_t  nClass;        // 物品分类
    int32_t nCount;        // 数量
    int8_t  nCostType;     // 消耗类型 (0=无, 1=铜钱, 2=元宝, 3=金币)
    int32_t costValue;     // 消耗值
    int8_t  bindType;      // 绑定类型
    int32_t endTime;       // 到期时间
    int64_t srcId;         // 来源装备ID
};

class CDropItem : public StaticObj
{
public:
    CDropItem();
    virtual ~CDropItem();

    virtual void reset() override;
    virtual EntityId_t getId() const override { return m_mid; }

    bool appendInfo(Answer::NetPacket *packet);
    bool checkOwner(CharId_t nCharId, int32_t nTeamId);
    int32_t pick(Player *player);
    bool checkEnd();

    EntityType getDropType() const { return m_entityType; }

private:
    bool        m_autoPick;
    bool        m_bPicked;
    int32_t     m_connid;
    int8_t      m_droperType;
    int64_t     m_endTick;
    int64_t     m_freeTick;
    DropItemData m_item;
    int64_t     m_mid;
    std::string m_name;
    CharId_t    m_owner;
    int64_t     m_showTick;
    int32_t     m_teamId;
};

#endif // _CDROPITEM_H_
