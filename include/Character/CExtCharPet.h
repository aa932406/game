// CExtCharPet.h
#ifndef CEXTCHARPET_H
#define CEXTCHARPET_H

#include "CExtSystemBase.h"
#include "Common/CommonTypes.h"
#include <list>

class CObjPet;

class CExtCharPet : public CExtSystemBase
{
public:
    CExtCharPet();
    virtual ~CExtCharPet();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    int32_t onFight(Answer::NetPacket* inPacket);
    int32_t onRest(Answer::NetPacket* inPacket);
    int32_t onUpStar(Answer::NetPacket* inPacket);
    int32_t onDressEquip(Answer::NetPacket* inPacket);
    int32_t onUnDressEquip(Answer::NetPacket* inPacket);
    int32_t onSkillUpLevel(Answer::NetPacket* inPacket);
    int32_t onChangeName(Answer::NetPacket* inPacket);
    int32_t onChangeAi(Answer::NetPacket* inPacket);
    int32_t onPetHuanHua(Answer::NetPacket* inPacket);
    int32_t onFitting(Answer::NetPacket* inPacket);
    
    bool Fight();
    void onRestPet();
    void SendPetStaus();
    void OnLevelUp();
    bool IsFunctionOpen();
    void OnInitPet();
    void OnPlayerDie();
    void clearSummonPet();
    void OnSummon(int32_t nSkillId, const void* pos);
    bool HaveSummonAlive();
    void AddCharAttr();
    void AddPetAttr(void* pObjPet);
    void RecalPetAttr();
    void AddTarget(void* launcher, bool bForce);
    void SetTargetTile(int32_t tx, int32_t ty, Direction dir);
    void SendPetInfo();
    
private:
    void sendPetInfo();
    bool addSummonPet(void* pPet);
    void removeSummonPet(void* pPet);
    
    struct SummonInfo
    {
        int32_t nTalent;
        std::list<void*> lPets;
    };
    
    CObjPet* m_pFightPet;
    int32_t m_nStar;
    int32_t m_HuanHua;
    int32_t m_nLastTick;
    void* m_vSummonPet[4];
    std::list<SummonInfo> m_lSummons;
};

#endif