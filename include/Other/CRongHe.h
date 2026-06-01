#ifndef _CRONGHE_H_
#define _CRONGHE_H_

#include "Character/CExtSystemBase.h"

class CRongHe : public CExtSystemBase
{
public:
    CRongHe();
    virtual ~CRongHe();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    AttrAddonList GetAttrList(int32_t nSlot);
    int32_t OnRongLian(Answer::NetPacket* inPacket);
    int32_t OnEquipRongLian(Answer::NetPacket* inPacket);
    int32_t OnOneKeyItemRecovery(Answer::NetPacket* inPacket);
    int32_t OnItemRecovery(Answer::NetPacket* inPacket);
    int32_t OnDismantlingEquip(Answer::NetPacket* inPacket);
    void SendAllRongHeInfo();
    void SendOneRongHeInfo(int32_t nSlot);
    void SendRongHeResult(int32_t nId, int32_t nSuccess, MemChrBag stu);
    std::string GetRongHeEquipString();
    void ParesRongHeEquipString(std::string* p_RongHeEquipString);
};

#endif // _CRONGHE_H_

