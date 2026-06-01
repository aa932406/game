#ifndef __PLANT_ACTIVITY_H__
#define __PLANT_ACTIVITY_H__

#include "Game/Plant.h"
#include <cstdint>

class CActivityMap;
class Player;
class CfgPlant;
class CfgMapPlant;

class PlantActivity : public Plant
{
public:
    PlantActivity();
    virtual ~PlantActivity();

    // 初始化
    void init(CActivityMap* pActivityMap,
              const CfgPlant* const cfgPlant,
              const CfgMapPlant* const cfgMapPlant,
              int32_t nLifeTime);
    
    // 采集
    int32_t onBeginGather(Player* player);
    int32_t onEndGather(Player* player);
    
    // 生命周期
    bool IsExpired() const;
    int32_t GetLeftTime() const;
    void SetLifeTime(int32_t lifeTime);
    
    // 定时器
    void OnTimer(int64_t curTick);
    
    // Getter
    CActivityMap* GetActivityMap() const;
    
    int32_t getPlantId() const { return m_nPlantId; }
    void setPlantId(int32_t id) { m_nPlantId = id; }

protected:
    CActivityMap* m_pActivityMap;
    int32_t m_nLifeTime;
    int32_t m_nPlantId;
};

#endif // __PLANT_ACTIVITY_H__