#ifndef _STATICOBJ_H_
#define _STATICOBJ_H_

#include "Common/CommonTypes.h"
#include "Common/EntityType.h"
#include "Utility/Position.h"

// 前置声明
class Map;

class StaticObj
{
public:
    StaticObj();
    StaticObj(EntityType entitytype);
    virtual ~StaticObj();

    // 重置
    virtual void reset();

    // 获取类型
    EntityType getType() const { return m_entityType; }
    
    // 获取ID (虚函数，由子类实现)
    virtual EntityId_t getId() const { return 0; }

    // 地图相关
    int32_t getMapId() const;
    int32_t getMapType() const;
    bool InDungeon() const;
    bool InActivity() const;
    bool InPrison() const;
    int32_t GetRunnerId() const;
    bool IsSpecialBossMap() const;
    bool IsCycleTower() const;
    bool IsKuangDong() const;
    bool IsTerritory() const;

    // 获取位置
    Position getCurrentTile() const { return m_currentTile; }
    Position getCurrentPos() const { return m_currentPos; }
    int32_t GetPosX() const { return m_currentPos.x; }
    int32_t GetPosY() const { return m_currentPos.y; }
    Map* getMap() const { return m_pMap; }

protected:
    EntityType m_entityType;    // 实体类型
    Position   m_currentPos;    // 当前像素位置
    Position   m_currentTile;   // 当前格子位置
    Map*       m_pMap;          // 所在地图
};

#endif // _STATICOBJ_H_