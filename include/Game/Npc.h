#ifndef _NPC_H_
#define _NPC_H_

#include "Common/CommonTypes.h"
#include "Game/Unit.h"

// 前置声明
struct CfgDungeonNpcCost;

// Npc 继承自 Unit (从反编译代码确认: Unit::Unit(this, EntityType::ET_NPC))
class Npc : public Unit
{
public:
    Npc();
    ~Npc();

    void reset();
    bool isLifeEnd() const;
    void onTalkByPlayer(Player *player);
    void init(const CfgNpc *cfgNpc, int32_t EndTime);
    bool appendInfo(Answer::NetPacket *packet);
    Position GetStallPos();
    bool CheckcheckPlatform(const std::string *platform);
    CfgDungeonNpcCost GetNpcCost(int8_t choice);

private:
    // 虚函数表指针
    void** _vptr_Entity;
    // 配置类型已确认: CfgNpc* (CfgNpc::CfgNpc(&this->m_cfgNpc))
    const CfgNpc* m_cfgNpc;
    int64_t m_endTime;
    int32_t m_nOwner;
};

#endif // _NPC_H_
