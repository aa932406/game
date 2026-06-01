#include "Activity/NpcActivity.h"
#include "Config/CfgNpc.h"
#include "Game/CTimer.h"
#include "Game/Player.h"

NpcActivity::NpcActivity()
    : m_activityId(0)
    , m_endTime(0)
{
}

NpcActivity::~NpcActivity()
{
}

void NpcActivity::init(const CfgNpc* const cfgNpc, int32_t activity_id, int32_t lifetime)
{
    if (!cfgNpc) return;

    m_activityId = activity_id;

    if (lifetime > 0)
    {
        CTimer* timer = CTimer::GetInstance();
        m_endTime = lifetime + timer->GetNow();
    }
}

void NpcActivity::reset()
{
    m_activityId = 0;
    m_endTime = 0;
}

bool NpcActivity::isLifeEnd()
{
    if (m_endTime <= 0) return false;

    CTimer* timer = CTimer::GetInstance();
    return timer->GetNow() >= m_endTime;
}

int64_t NpcActivity::getNow(void* obj)
{
    CTimer* timer = CTimer::GetInstance();
    return timer->GetNow();
}

bool NpcActivity::OnInteract(Player* player)
{
    if (!player) return false;
    // 默认交互行为，由子类重写
    return false;
}

void NpcActivity::OnTimer(int64_t curTick)
{
    // 检查是否过期
    if (isLifeEnd())
    {
        // NPC过期，由地图管理器处理移除
    }
}

int32_t NpcActivity::GetActivityId() const
{
    return m_activityId;
}

int32_t NpcActivity::GetEndTime() const
{
    return m_endTime;
}

void NpcActivity::SetEndTime(int32_t endTime)
{
    m_endTime = endTime;
}
