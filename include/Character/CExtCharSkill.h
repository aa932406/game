// CExtCharSkill.h
#ifndef CEXTCHARSKILL_H
#define CEXTCHARSKILL_H

#include "CExtSystemBase.h"
#include <string>
#include <map>
#include <list>

struct SkillLevelInfo
{
    int32_t Level;
    int32_t Point;
};

struct AddonSkill
{
    int32_t nSkillId;
    int32_t nSkillFlag;
    int64_t nEndTick;
    int32_t nTrigTimes;
};

struct ActiveSkill
{
    int32_t nSkillId;
    int64_t nEntityId;
    int32_t nType;
};

struct MemTalent
{
    int32_t nId;
    int32_t vAddon[9];
};

class CExtCharSkill : public CExtSystemBase
{
public:
    CExtCharSkill();
    virtual ~CExtCharSkill();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onAddTalentPoint(Answer::NetPacket* inPacket);
    int32_t onDoUnitSkill(Answer::NetPacket* inPacket);
    int32_t onDoAddonSkill(Answer::NetPacket* inPacket);
    
    bool CanUseSkillAndMove(int64_t nCurTick, bool ResetTick);
    void AddCharAttr();
    void SendTalentInfo();
    void SendTalentAddon();
    void CheckHPPecentTrigBuff(int32_t nLastPecent, int32_t nNowPecent);
    void CheckPhaseDamageTrigBuff(int8_t nPhase);
    void CheckSelfTrigBuff(int8_t nTrigType);
    void CheckSummonBuff();
    void initSkills(int32_t TalentId, int32_t TalentLevel);
    void removeSkillByTalent(int32_t TalentId, int32_t TalentLevel);
    void checkActiveList();
    void activeSkill(int32_t nId, const void* handler);
    void doSkillActive(const void* pCfgSkill, void* pTarget);
    void AddPower(int32_t nValue);
    bool addAddonSkill(const AddonSkill* addonSkill);
    bool removeAddonSkill(int32_t nSkillFlag);
    void checkAddonSkills(int64_t nCurTick);
    void checkSummonSkills(int64_t nCurTick);
    void checkSuitSkillEnergy(int64_t nCurTick);
    int32_t calActiveSkillCD(int32_t nSkillId);
    void recalTalentAddon();
    void getTalentAddon(void* addonList);
    void addTalentAddon(const void* stu);
    void recalAcviveSkill();
    void addActiveSkill(const void* pCfgSkill);
    void recalDropMoneyRate();
    void AddSkillPoint(int32_t Id, int32_t Points);
    bool UseSkillBook(int32_t Id);
    bool AddSkillBookPoint(int32_t Id, int32_t Points);
    void SendlearedSkill(int32_t TalentId);
    void AddOtherSkill(int32_t TalentId, int32_t TalentLevel);
    void CallSkillBattle();
    void AddSuitSkillEnergy();
    void checkSkillTrigBuff(const void* pCfgSkill, void* results);
    int32_t doSkill(Answer::NetPacket* inPacket, const void* pCfgSkill, void* results, 
                    void* srcPos, int32_t nSkillId, int32_t nSkillFlag, int32_t bCheck);
    void addSkillBuffTo(int32_t nBuffId, int32_t nSkillId, int32_t nGroupId, void* pTarget);
    
    void InitSystem() override;
    
private:
    std::string saveTalentPoints();
    void loadTalentPoints(const std::string* str);
    const void* getActiveSkill(int32_t nSkillId);
    const void* getAddonSkill(int32_t nSkillFlag);
    
    std::map<int32_t, SkillLevelInfo> m_SkillLevelMap;
    std::list<AddonSkill> m_lAddonSkills;
    int64_t m_nLastAddonTick;
    int64_t m_PowerLastTick;
    int64_t m_lastActionTick;
    int64_t m_nLastEnergyTick;
    std::map<int32_t, int64_t> m_mSkillCD;
    std::list<int32_t> m_lActiveSkills;
    std::list<int32_t> m_lPassiveSkills;
    std::list<int32_t> m_lTrigSkills;
    int32_t m_nPower;
    int32_t m_nDropMoneyRate;
    std::list<void*> m_lSummonSkills;
    std::map<int32_t, void*> m_mActiveSkills;
    std::list<MemTalent> m_lActiveTalent;
    int32_t m_SkillBattle;
    std::map<int32_t, int32_t> m_OthreSkillLevelMap;
    int64_t m_SkillPublicCD;
    std::list<ActiveSkill> m_lActive;
};

#endif