#ifndef _CCHRDEPOT_H_
#define _CCHRDEPOT_H_

#include "Character/CExtSystemBase.h"
#include "Character/CExtCharBag.h"
#include <string>
#include <vector>

class CChrDepot : public CExtSystemBase
{
public:
    CChrDepot();
    virtual ~CChrDepot();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnDepotOpr(Answer::NetPacket* inPacket);
    int32_t OnDepotPwdCheck(Answer::NetPacket* inPacket);
    int32_t OnDepotPwdSetting(Answer::NetPacket* inPacket);
    int32_t OnDepotSwitch(Answer::NetPacket* inPacket);
    int32_t OnDepotInfo(Answer::NetPacket* inPacket);
    int32_t OnDepotPLock(Answer::NetPacket* inPacket);
    void SendDepotItems();
    void SendDepotInfo();
    int32_t GetDepotCellNum();
    int32_t GetDepotExpandCount();
    bool CheckPwd(std::string* pwd);
    void SetDepotPwd(std::string* pwd);
    bool ItemCanEnterDepot(ItemInfo* item);
    void ExpandDepotCell();
    void SetDepotLockState(bool bLock);

private:
    CExtCharBag m_DepotBag;
    std::string m_DepotPwd;
    int32_t m_nDepotCellCount;
    bool m_bDepotLock;
    bool m_bPwdCheckPass;
};

#endif // _CCHRDEPOT_H_
