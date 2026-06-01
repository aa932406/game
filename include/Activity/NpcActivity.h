#ifndef __NPC_ACTIVITY_H__
#define __NPC_ACTIVITY_H__

#include "Game/Npc.h"
#include <cstdint>

class Player;
class CfgNpc;

class NpcActivity : public Npc
{
public:
    NpcActivity();
    virtual ~NpcActivity();

    // 初始化与清理
    void init(const CfgNpc* const cfgNpc, int32_t activity_id, int32_t lifetime);
    void reset();
    
    // 生命周期检查
    bool isLifeEnd();
    int64_t getNow(void* obj);
    
    // 交互
    bool OnInteract(Player* player);
    
    // 定时器
    void OnTimer(int64_t curTick);
    
    // Getter/Setter
    int32_t GetActivityId() const;
    int32_t GetEndTime() const;
    void SetEndTime(int32_t endTime);

protected:
    int32_t m_activityId;
    int32_t m_endTime;
};

#endif // __NPC_ACTIVITY_H__