#ifndef _COBJCARRIER_H_
#define _COBJCARRIER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Game/Unit.h"
#include "Utility/UnitHandle.h"

class Player;
class Map;
class Monster;
class Npc;
class CfgActiveSkill;
class CfgData;
class Answer_NetPacket;
class Position;

class CObjCarrier : public Unit
{
public:
    CObjCarrier();
    ~CObjCarrier();

    bool refresh();
    UnitHandle getHandle() const;
    CharId_t getOwner() const;
    int32_t getLevel() const;
    int32_t getBattle() const;
    bool isFriendSide(Unit *pUnit);
    bool checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit);
    void reset();
    void Init(Player *pOwner, int32_t nId);
    bool appendInfo(Answer::NetPacket *packet);
    bool HaveSkill(int32_t nSkillId) const;
    void initBaseAttr();

private:
    int32_t m_nId;
    Player *m_pPlayer;
    std::list<int32_t> m_lSkills;
    bool m_needSync;
    bool m_needSyncAttr;
    bool m_needSyncSelf;
};

#endif // _COBJCARRIER_H_