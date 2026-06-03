#ifndef _CCHARWUHUN_H_
#define _CCHARWUHUN_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Database/PlayerDBData.h"
#include "Character/CExtSystemBase.h"
class Player;
class GameService;
class WuHunItem;
class CreateWuHun;
class GameService;

class CCharWuHun : public CExtSystemBase
{
public:
    CCharWuHun();
    virtual ~CCharWuHun();

    virtual void Init(Player* pPlayer) override;
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;

    void AddCharAttr();
    int32_t onAskWuHunInfo(Answer::NetPacket* inPacket);
    int32_t onDressWuHun(Answer::NetPacket* inPacket);
    int32_t onUnDressWuHun(Answer::NetPacket* inPacket);
    void OnRemoveTalent(int32_t TalentId, int32_t TalentLevel);
    void OnAddTalent(int32_t TalentId, int32_t TalentLevel);
    int32_t onCreateWuHun(Answer::NetPacket* inPacket);
    void sendWuHunInfo();
    void sendWuHunSlotInfo(int32_t Level, int32_t Slot);
    bool checkWuHunPlace(int32_t Type, int32_t Slot);
    int32_t GetSuitLevel(int32_t nLevel);

private:
    Player* m_pPlayer;
    int32_t m_WuHun[5][16];
};

#endif // _CCHARWUHUN_H_
