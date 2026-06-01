#include "Other/SkillBuff.h"

#include "Game/Unit.h"
#include "Game/GameService.h"
#include "Game/StaticObj.h"
#include "Config/CfgBuff.h"

SkillBuff::~SkillBuff()
{
}

void SkillBuff::interval()
{
    int32_t attackState = 0;
    int32_t hpchange = 0;
    int32_t nDamage = 0;
    int32_t nRealDamage = 0;

    if (m_cfgBuff->beneficial)
    {
        if (m_cfgBuff->modify > 0)
            hpchange = m_cfgBuff->modify * (int64_t)m_nAtkMax / 10000;
        if (m_cfgBuff->hpValue > 0)
            hpchange += m_cfgBuff->hpValue;
        if (m_cfgBuff->hpPecent > 0)
            hpchange += m_cfgBuff->hpPecent * m_unit->getMaxHp() / 100;
        m_unit->addHp(hpchange);
    }
    else
    {
        if (m_cfgBuff->modify > 0)
            nDamage = m_cfgBuff->modify * (int64_t)m_nAtkMax / 10000;
        if (m_cfgBuff->hpValue > 0)
            nDamage += m_cfgBuff->hpValue;
        if (m_cfgBuff->hpPecent > 0)
            nDamage += m_cfgBuff->hpPecent * m_unit->getMaxHp() / 100;
        nRealDamage = m_unit->struckDamage(nDamage, &attackState, m_cfgBuff->attack_type, m_launcher, m_Mid);
        hpchange -= nRealDamage;
    }

    if (hpchange)
        broadcastIntervalEffect(hpchange, attackState);
}

void SkillBuff::broadcastIntervalEffect(int32_t hpchange, int32_t attackState)
{
    Map *pMap = StaticObj::getMap(m_unit);
    if (pMap)
    {
        Answer::NetPacket *packet = GameService::instance()->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2738);
        if (packet)
        {
            packet->writeInt32(0);
            packet->writeInt16(m_skillid);
            packet->writeInt64(m_launcher.id);
            packet->writeInt8(m_launcher.type);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt16(StaticObj::getCurrentTile(m_unit).x);
            packet->writeInt16(StaticObj::getCurrentTile(m_unit).y);
            packet->writeInt8(0);
            packet->writeInt16(1);
            packet->writeInt64(m_unit->getId());
            packet->writeInt8(m_unit->getType());
            packet->writeInt32(hpchange);
            packet->writeInt16(attackState);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            packet->writeInt32(0);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::instance()->worldBroadcast(packet);
        }
    }
}
