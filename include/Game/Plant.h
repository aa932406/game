#ifndef _PLANT_H_
#define _PLANT_H_

#include "Common/CommonTypes.h"
#include "Other/StaticObj.h"

// Plant 继承自 StaticObj (从反编译代码确认: StaticObj::StaticObj(this, EntityType::ET_PLANT))
class Plant : public StaticObj
{
public:
    Plant();
    ~Plant();

    void init(Map* pMap, const CfgPlant* pCfgPlant, const CfgMapPlant* pCfgMapPlant, int32_t nLifeTime);
    void reset();
    void refresh();
    int32_t GetReviveTime();
    int32_t onBeginGather(Player *player);
    int32_t onEndGather(Player *player);
    int32_t PlantEvent(Player *pPlayer, int32_t EventId);
    void SendGainItem(Player *pPlayer, MemChrBagVector *vItem);
    int32_t onBreakGather(Player *player);
    void BreakGather();
    bool IsShow();
    bool appendInfo(Answer::NetPacket *packet);
    void BroadcastChange();
    void BroadcastGathered();
    void broadcastRemove();
    int32_t getNow();
    void getLocalNow(tm *retstr);
    bool isLifeEnd();
    void setLifeTime(int32_t nLifeTime);
    void revive();
    int32_t GetSpecialItem(int32_t nIndex);

private:
    // 虚函数表指针
    void** _vptr_Entity;
    // 成员变量 (从反编译代码还原)
    int8_t m_Camp;
    const CfgMapPlant* m_cfgMapPlant;        // 配置类型已确认: CfgMapPlant*
    const CfgPlant* m_cfgPlant;              // 配置类型已确认: CfgPlant* (CfgPlant::CfgPlant(&this->m_cfgPlant))
    int64_t m_corpseTick;
    Player* m_gather;                        // 正在采集的玩家 (this->m_gather = 0)
    int64_t m_gatherTick;
    bool m_isCorpse;                         // (this->m_isCorpse = 0)
    bool m_isHide;
    int64_t m_nLifeTime;
    Map* m_pMap;
    EntityId_t m_pSummonBoss;                // (this->m_pSummonBoss = 0)
    int32_t m_whoCanPlant;
};

#endif // _PLANT_H_