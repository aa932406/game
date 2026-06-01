#ifndef _CSPECIALEQUIP_H_
#define _CSPECIALEQUIP_H_

#include "Character/CExtSystemBase.h"

class CSpecialEquip : public CExtSystemBase
{
public:
    CSpecialEquip();
    virtual ~CSpecialEquip();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    void AddCharAttr();
    void InitSkill();
    bool IsActivation(int32_t Day);
    void GetSpecialEquipIcon(IconStateList* IconList);
    void SendSpecialEquipIcon();
    int32_t CanActivationCount();
    void GetSpecialEquipIcon(ShowIcon* __return_ptr retstr);
    int32_t OnActivation(Answer::NetPacket* inPacket);
    bool CanActivation(int32_t nIndex);
    void SendActivationInfo();
};

#endif // _CSPECIALEQUIP_H_


