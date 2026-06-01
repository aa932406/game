#ifndef _COBJPET_H_
#define _COBJPET_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Utility/UnitHandle.h"

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;
class CfgPetData;
class CfgSkillTable;
class CfgTalentTable;
class CfgPetEquipTable;
class CfgTalent;
class CfgActiveSkill;

// 宠物状态枚举
enum class PET_STATE
{
    PS_REST = 0,        // 休息
    PS_FIGHTING = 1,    // 战斗
    PS_FITTING = 2,     // 附体
};

// 宠物战斗状态枚举
enum class PET_FIGHT_STATE
{
    PFS_STAND = 0,          // 站立
    PFS_FOLLOW = 1,         // 跟随
    PFS_ATTACK = 2,         // 攻击
    PFS_WALK_ATTACK = 3,    // 走砍
    PFS_AWAY_FROM_MAP = 4,  // 远离地图
};

// 宠物技能信息
struct PetSkillInfo
{
    int32_t nId;
    int32_t nLevel;
    int64_t nCDTime;
};

// 宠物技能附加信息
struct SkillAddonInfo
{
    int32_t nSkillId;
    UnitHandle nTarget;
    int64_t nLastTime;
    int64_t nInterval;
};

class CObjPet : public Unit
{
public:
    CObjPet();
    virtual ~CObjPet();

    virtual bool refresh();
    CharId_t getOwner() const;
    int32_t getLevel() const;
    int32_t getBattle() const;
    bool isFriendSide(Unit *pUnit);
    bool checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit);
    void postDamage(int32_t damge, UnitHandle launcher, int32_t Mid);
    virtual void reset();
    void InitSummon(Player *pOwner, int32_t nBaseId, int32_t nSkillId);
    void ReplyHp(int64_t CurTick);
    bool appendInfo(Answer::NetPacket *packet);
    int32_t GetBaseId() const;
    void CheckSkillOpen(int32_t nStar);
    void Die();
    bool CheckRelive();
    void SendPetStatus();
    bool AppendPetInfo(Answer::NetPacket *packet);
    void appendAttrInfo(Answer::NetPacket *packet);
    void appendEquipInfo(Answer::NetPacket *packet);
    void appendSkillInfo(Answer::NetPacket *packet);
    PetSkillInfo* GetSkill(int8_t nSkillPos);
    void LearnSkill(int8_t nSkillPos);
    void ChangeName(const std::string *strName);
    void ChangeAi(int8_t State);
    std::string GetName() const;
    void LeaveMap();
    void EnterMap();
    void EnterMap(const Position *pos);
    void OnLogout();
    void checkSwitchMap();
    void OnLevelUp();
    int8_t calTargetPriority(const UnitHandle *target);
    void AddTarget(const UnitHandle *target, bool bForce);
    void ResetTargetTile(int32_t tx, int32_t ty, Direction dir);
    void backToPlayer();
    Position getFollowTile(int32_t tx, int32_t ty, Direction dir) const;
    void recalAttr();
    void initBaseAttr();
    void addSystemAttr();
    void addPassiveSkillAttr();
    void AddEquipAttr(Unit *pTarget);
    int32_t GetSkillBattle() const;
    void sendPetLearnSkill(int32_t nSkill);
    void broadcastPetSkillLevelUp(int16_t nSkillId, int16_t nSkillLevel);
    void OnLoadFromDB(const struct DBPet *pet, int32_t nStar);
    void Init(Player *pOwner, PetId_t nPetId, int32_t nBaseId, bool bAlive, PET_STATE state, Direction nDir, int8_t AiState);
    void OnSaveToDB(struct DBPet *pet);
    void updateFightState();
    void setFightState(PET_FIGHT_STATE state);
    void onStand();
    void onFollow();
    void changeTarget();
    void onAttack();
    void onWalkAttack();
    void checkFarAway();
    void checkSpeed();
    int32_t calAddBattle(int32_t nIndex) const;
    void addEquipAttr();
    void addSummonAttr();
    void addOtherAttr() { /* TODO: implement from pseudocode */ }
    int32_t GetEquip(int8_t nPos);
    void SetEquip(int8_t nPos, int32_t nEquipId);
    void refreshSkill(int64_t nCurTick);
    void ClearTarget();
    void clearTarget();
    bool AliveFlag();
    bool IsFighting();
    bool IsSummonPet();
    int32_t getReliveTime();
    void setStand();
    void onDie();
    Player* GetPlayer() const { return m_pPlayer; }
    PetId_t GetPetId() const { return m_nPetId; }
    int32_t getMoveSpeed() const;
    int32_t getHpPercent() const;
    int32_t getHp() const;
    int32_t getMaxHp() const;
    Map* getMap() const { return m_pMap; }

private:
    int32_t m_AiState;
    bool m_bAlive;
    std::list<SkillAddonInfo> m_lAddonSkill;
    int64_t m_nDieTick;
    int64_t m_nDieTime;
    int32_t m_nFightState;
    int64_t m_nLastReplyHp;
    int64_t m_nLastSkillTick;
    int32_t m_nPetId;
    Direction m_nStandPlace;
    int32_t m_nState;
    int32_t m_nSummonSkillId;
    const CfgPetData *m_pCfgPet;
    Map *m_pMap;
    PetSkillInfo *m_pNextSkill;
    Player *m_pPlayer;
    char m_strName[32];
    UnitHandle m_target;
    int32_t m_vEquip[4];
    PetSkillInfo m_vSkill[9];
};

#endif // _COBJPET_H_
