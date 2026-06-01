#include "Activity/PlantActivity.h"
#include "Activity/CActivityMap.h"
#include "Config/CfgPlant.h"
#include "Config/CfgMapPlant.h"
#include "Game/Player.h"
#include "Map/Map.h"
#include "Game/CTimer.h"
#include "Other/Logger.h"

PlantActivity::PlantActivity()
    : m_pActivityMap(nullptr)
    , m_nLifeTime(0)
{
}

PlantActivity::~PlantActivity()
{
}

void PlantActivity::init(CActivityMap* pActivityMap,
                         const CfgPlant* const cfgPlant,
                         const CfgMapPlant* const cfgMapPlant,
                         int32_t nLifeTime)
{
    if (!pActivityMap) return;
    
    m_pActivityMap = pActivityMap;
    
    if (nLifeTime > 0)
    {
        CTimer* timer = CTimer::GetInstance();
        m_nLifeTime = nLifeTime + timer->GetNow();
    }
    
    Plant::init(pActivityMap, cfgPlant, cfgMapPlant, 0);
}

int32_t PlantActivity::onBeginGather(Player* player)
{
    if (!player) return -1;
    
    int32_t err = Plant::onBeginGather(player);
    if (err != 0) return err;
    
    if (m_pActivityMap)
    {
        return m_pActivityMap->onBeginGather(this, player);
    }
    
    return 0;
}

void PlantActivity::onEndGather(Player* player)
{
    if (!player) return;
    
    int32_t err = Plant::onEndGather(player);
    if (err != 0) return;
    
    if (m_pActivityMap)
    {
        m_pActivityMap->onPlantGather(this, player);
    }
}

bool PlantActivity::IsExpired() const
{
    if (m_nLifeTime <= 0) return false;
    
    CTimer* timer = CTimer::GetInstance();
    return timer->GetNow() >= m_nLifeTime;
}

int32_t PlantActivity::GetLeftTime() const
{
    if (m_nLifeTime <= 0) return 0;
    
    CTimer* timer = CTimer::GetInstance();
    int32_t now = timer->GetNow();
    
    if (now >= m_nLifeTime) return 0;
    
    return m_nLifeTime - now;
}

void PlantActivity::SetLifeTime(int32_t lifeTime)
{
    m_nLifeTime = lifeTime;
    Plant::setLifeTime(lifeTime);
}

void PlantActivity::OnTimer(int64_t curTick)
{
    Plant::OnTimer(curTick);
    
    // 检查是否过期
    if (IsExpired())
    {
        // 植物过期，从地图移除
        if (m_pMap && m_pActivityMap)
        {
            m_pMap->removePlant(this);
        }
    }
}

CActivityMap* PlantActivity::GetActivityMap() const
{
    return m_pActivityMap;
}
