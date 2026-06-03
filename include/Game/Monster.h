#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "Common/CommonTypes.h"
#include "Game/Unit.h"
#include "Utility/UnitHandle.h"

// Monster 继承自 Unit (从反编译代码确认: Unit::Unit(this, EntityType::ET_MONSTER))
class Monster : public Unit
{
public:
    Monster();
    ~Monster();

    bool refresh();
    void ChangeOwner();
    void getItem(MemChrBag *retstr);
    void reset();
    void addDamager(CharId_t cid);
    Position getRunPosition(Position *tarPos, int32_t nRunDistance);
    void SetOwner(CharId_t Cid, std::string *p_Name);
    void postDamage(int32_t damge, UnitHandle launcher, int32_t Mid);
    void SetGroupMonster(int32_t MaxRegion, int32_t InRegion);
    void SetRevivePos(std::vector<Position> *p_PosVector);
    Position GetGroupMonsterPos();
    Position GetNearestPos();
    void SetNeedSyncAround();
    void SyncAround();
    void OnLevelUp(CfgMonster *cfgMonster);
    bool appendInfo(Answer::NetPacket *packet);
    void Kill(Player *pKiller);
    Position getBornPosition() const;
    int32_t getReviveTime() const;
    bool needDel() const;
    void destroy();
    void getKiller();
    bool isActivity(int32_t Type, int32_t *setValue);
    bool isActivity(int32_t Type);
    void SetFamilyId(FamilyId_t fid, std::string *p_strFamilyName);
    void broadcastIntoMap();
    void updateState();
    void onStand();
    void onWalkAround();
    void onRunAround();
    bool AnyAttacks(EntityId_t id);
    void onWalkAttack();
    void onWalkHome();
    UnitHandle randHandle(EntityId_t id);
    void onAttack();
    void onEscape();
    void setEscapeTarget();
    bool checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit);
    void onCorpse();
    void onGhost();
    void OnRevive();
    bool IsShow() const;
    void onRunOnRoad();
    bool needSync();
    void setState(MonsterState state);
    static void setState(Monster* p, MonsterState state) { if (p) p->setState(state); }
    void onSendKillerReward(Player *pPlayer);
    void GetOwner();
    int32_t CanAddBossSocre(Player *pPlayer, int32_t nAddValue);
    void SetLifeTime(int64_t lifeEndTime) { m_nLifeTime = lifeEndTime; }
    void ClearTarget() { m_target = UnitHandle(); }
    const CfgMonster& GetCfgMonster() const { return *m_cfgmonster; }
    int32_t GetLevel() const;
    int32_t GetMonsterId() const;
    MonsterState GetState() const { return m_state; }
    void die();
    void ActDrop(DropItemVector *dropItems);
    void revive();
    int32_t getMinute();
    void refreshSkill(int32_t HPPecent);
    bool isSkillCD(SkillId_t nSkillId);
    void setSkillCD(SkillId_t nSkillId, int32_t nCDTime);
    void flyHome();
    void backHome();
    void checkHPRevive();
    void checkLifeTime();
    void resetSkill();
    void resetRandAttr();
    void updatePlayerDamage(Player *player, int32_t damage);
    void checkEnterBossRegion();
    void checkLeaveBossRegion();
    void addPlayer(Player *player, int32_t damage);
    void updataBossDamageInfo(Player *player);
    void SendAllDamageRank(Player *player);
    void sendScorePlayerInfo(Player *player, int8_t status);
    void sendAllRankInfo();
    void refreshRank();
    void setKillerBossPalyer();
    bool needCheckBossRank();
    void SendAllDamageRank();
    void UpdateRegionPlayer();
    bool checkTeamClear(int32_t nTeamId) const;
    void clearTeam(const Int32List *tList);
    void onPlayerDie(Player *player);
    void onTeamUpdate(Player *player);
    void setDelFlg();
    void MonsterDamageDrop(Player *pPlayer);
    int32_t getEscapeOdds();
    bool updatePosition();
    int32_t GetMonsterCfgReviveTime();
    void AddDiligenceDrop(Player *pPlayer, DropItemVector *dropItems);
    void AddHP(int32_t nAddValue);
    void FillHP(int32_t nPercent);

private:
    // 成员变量 (从反编译代码的构造和析构还原)
    std::map<long, ActStruct> m_ActStructMap;
    bool m_ChangeOwner;
    bool m_DamageFlag;
    int8_t m_DieType;
    bool m_HaveItem;
    int32_t m_InRegion;
    bool m_IsGroupMonster;
    int32_t m_MaxRegion;
    int32_t m_MonsterHp;
    bool m_NeedSyncAround;
    int32_t m_OwnerId;
    int64_t m_OwnerLastAttack;
    std::string m_OwnerName;
    std::map<long, PlayerScore> m_PlayerScore;
    std::vector<Position> m_RevivePos;
    std::multiset<PlayerScore*> m_ScoreRank;
    std::map<int, PlayerScore> m_TeamScore;
    bool m_TunShi;
    int64_t m_attackTick;
    int64_t m_attackTime;
    bool m_bHPRevive;
    const CfgMonster* m_cfgMonsterAI;
    const CfgMonster* m_cfgSpecialMonster;
    const CfgMapMonster* m_cfgmapmonster;
    const CfgMonster* m_cfgmonster;
    int64_t m_corpseTick;
    bool m_delFlag;
    int64_t m_ghostTick;
    UnitHandle m_killer;
    std::list<CharId_t> m_lDamager;
    std::list<int> m_lRandType;
    int64_t m_lastActionTick;
    int32_t m_lastUpdateMinute;
    int32_t m_nActivityId;
    int32_t m_nEscapeDirection;
    int64_t m_nEscapeTick;
    int32_t m_nFamilyId;
    int32_t m_nHPPecent;
    int64_t m_nLastCheckBossTick;
    int64_t m_nLastReviveHPTick;
    int64_t m_nLastSyncTick;
    int64_t m_nLifeTime;
    int64_t m_nReviveTime;
    int64_t m_nRunTick;
    const CfgActiveSkill* m_pCfgSkill;
    int64_t m_reviveTick;
    int64_t m_standTick;
    int64_t m_standTime;
    MonsterState m_state;
    std::string m_strFamilyName;
    UnitHandle m_target;
    std::map<long, UnitHandle> m_targetMap;
    std::vector<int> m_vSkills;
};

#endif // _MONSTER_H_