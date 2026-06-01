#include "Other/Buff.h"
#include "Game/Unit.h"
#include "Config/CfgBuff.h"
#include "Config/CfgData.h"

Buff::Buff()
    : m_buffId(0)
    , m_buffLevel(0)
    , m_special(0)
    , m_index(0)
    , m_cfgBuff(nullptr)
    , m_target(nullptr)
    , m_launcher(nullptr)
    , m_endTick(0)
    , m_lastIntervalTick(0)
    , m_nEffectTimes(0)
    , m_nShield(0)
    , m_nType(0)
{
}

Buff::Buff(int32_t buffId, int32_t buffLevel, Unit* target, Unit* launcher)
    : m_buffId(buffId)
    , m_buffLevel(buffLevel)
    , m_special(0)
    , m_index(0)
    , m_cfgBuff(CfgData::instance()->getBuff(buffId))
    , m_target(target)
    , m_launcher(launcher)
    , m_endTick(0)
    , m_lastIntervalTick(0)
    , m_nEffectTimes(0)
    , m_nShield(0)
    , m_nType(0)
{
    if (launcher)
    {
        m_launcherHandle = launcher->getHandle();
    }
    if (m_target)
    {
        m_endTick = m_target->getTick() + getDuration();
    }
}

Buff::~Buff()
{
    restore();
}

int64_t Buff::remainTick() const
{
    if (m_target)
    {
        return m_endTick - m_target->getTick();
    }
    return 0;
}

bool Buff::expire() const
{
    if (m_target)
    {
        int64_t tick = m_target->getTick();
        return tick > m_endTick;
    }
    return true;
}

int32_t Buff::getDuration() const
{
    if (m_cfgBuff)
        return m_cfgBuff->duration;
    return 0;
}

const CfgBuff* Buff::getCfgBuff() const
{
    return m_cfgBuff;
}

void Buff::effect()
{
    if (m_cfgBuff && m_target)
    {
        if (m_cfgBuff->shield > 0)
        {
            m_nShield += m_cfgBuff->shield;
        }
        if (m_cfgBuff->type > 0)
        {
            m_nType = m_cfgBuff->type;
        }
    }
}

void Buff::interval()
{
    if (m_cfgBuff && m_cfgBuff->interval > 0 && m_target)
    {
        if (m_cfgBuff->beneficial)
        {
            int32_t hpchange = m_cfgBuff->modify;
            if (hpchange != 0)
                m_target->addHp(hpchange);
        }
        else
        {
            int32_t nDamage = m_cfgBuff->modify;
            if (nDamage != 0)
            {
                int32_t attackState = 0;
                m_target->struckDamage(nDamage, &attackState, m_cfgBuff->attack_type, m_launcherHandle, 0);
            }
        }
    }
}

void Buff::restore()
{
    for (const auto& restore : m_restore)
    {
        if (m_target)
        {
            m_target->AddBuffAttrValue(restore.attrIndex, -restore.value);
        }
    }
    m_restore.clear();
}

bool Buff::effectTimes()
{
    ++m_nEffectTimes;
    if (m_cfgBuff)
        return m_nEffectTimes >= m_cfgBuff->effectTimes;
    return false;
}

bool Buff::shieldDamage(int32_t* nDamage)
{
    if (!nDamage || m_nShield <= 0) return false;

    if (*nDamage < m_nShield)
    {
        m_nShield -= *nDamage;
        *nDamage = 0;
        return true;
    }
    else
    {
        *nDamage -= m_nShield;
        m_nShield = 0;
        return false;
    }
}

void Buff::trigTime()
{
    if (m_cfgBuff && m_cfgBuff->interval > 0 && m_target)
    {
        if (m_target->getTick() - m_lastIntervalTick >= m_cfgBuff->interval)
        {
            interval();
            m_lastIntervalTick += m_cfgBuff->interval;
        }
    }
}

bool Buff::NeedShow() const
{
    if (m_cfgBuff)
        return m_cfgBuff->needShow;
    return true;
}
