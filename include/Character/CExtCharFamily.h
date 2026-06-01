// CExtCharFamily.h
#ifndef CEXTCHARFAMILY_H
#define CEXTCHARFAMILY_H

#include "CExtSystemBase.h"
#include <list>
#include <string>

struct FamilySkill
{
    int32_t nSkillId;
    int32_t nSkillLevel;
};

class CExtCharFamily : public CExtSystemBase
{
public:
    CExtCharFamily();
    virtual ~CExtCharFamily();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onContribution(Answer::NetPacket* inPacket);
    int32_t onOpenTerritory(Answer::NetPacket* inPacket);
    int32_t onEnterTerritory(Answer::NetPacket* inPacket);
    int32_t onFamilyLevelUpMedal(Answer::NetPacket* inPacket);
    int32_t onFamilyBuyHoe(Answer::NetPacket* inPacket);
    int32_t onWearHoe(Answer::NetPacket* inPacket);
    int32_t onSocialCreateFamily(Answer::NetPacket* inPacket);
    int32_t onSocialChangeFamilyName(Answer::NetPacket* inPacket);
    int32_t onSocialUpdateFamilyInfo(Answer::NetPacket* inPacket);
    int32_t onReceiveFamilyTask(Answer::NetPacket* inPacket);
    int32_t onQuickSubmitFamilyTask(Answer::NetPacket* inPacket);
    int32_t onGetFamilyTaskReward(Answer::NetPacket* inPacket);
    int32_t onImpeach(Answer::NetPacket* inPacket);
    int32_t onSocialImpeachResult(Answer::NetPacket* inPacket);
    int32_t onSendRedPacket(Answer::NetPacket* inPacket);
    int32_t onEnterFamilyDungeon(Answer::NetPacket* inPacket);
    int32_t onBindRedPacketTake(Answer::NetPacket* inPacket);
    int32_t onRequestSkillInfo(Answer::NetPacket* inPacket);
    int32_t onLearnSkill(Answer::NetPacket* inPacket);
    int32_t onSetTalkTask(Answer::NetPacket* inPacket);
    int32_t onGetTalkTaskReward(Answer::NetPacket* inPacket);
    int32_t onAddBossPoint(Answer::NetPacket* inPacket);
    int32_t onSocialStartBoss(Answer::NetPacket* inPacket);
    int32_t onSocialInviteAll(Answer::NetPacket* inPacket);
    int32_t onStartFamlyDungeon(Answer::NetPacket* inPacket);
    int32_t onGetFamilyDungeonInfo(Answer::NetPacket* inPacket);
    
    int32_t EndMining();
    int32_t EnterFamilyDungeon();
    bool IsDeclareWarFamily(FamilyId_t nFamilyId);
    void SendAddFamilyTaskCount();
    void sendSocialImpeach();
    void sendSocialSendRedPacket(int32_t nTotalValue, int32_t nTimes);
    void SendHoe();
    void sendSkillInfo();
    void AddSkillAttr();
    int32_t GetDefRdc(void* Target);
    void AddContribution(int32_t nValue, int32_t nReason);
    void StartFamilyAct(int32_t Param);
    bool AddMedRes(int32_t AddValues);
    void SendFamilyDungeonInfo();
    
    int32_t GetFamilyId();
    int32_t GetPosition();
    int32_t GetFamilyLevel();
    
private:
    int32_t getSkillLevel(int32_t nSkillId);
    void addSkillLevel(int32_t nSkillId);
    void loadSkillString(const std::string* strSkills);
    std::string saveSkillString();
    
    int32_t m_nFamilyId;
    int32_t m_nPosition;
    int32_t m_nContribution;
    int32_t m_nDailyContribution;
    int32_t m_MedLevel;
    int32_t m_MedRes;
    int32_t m_Hoe;
    int32_t m_EnterDungeionFamilyId;
    int32_t m_EnterCount;
    int32_t m_EnterTime;
    std::list<FamilySkill> m_lSkills;
};

#endif