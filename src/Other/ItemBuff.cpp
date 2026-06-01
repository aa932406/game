#include "Other/ItemBuff.h"

#include "Config/CfgData.h"
#include "Config/CfgBuff.h"
#include "Game/Unit.h"
#include "Utility/Answer.h"

ItemBuff::~ItemBuff()
{
}

int32_t ItemBuff::getGroupId()
{
    if (m_cfgItem)
        return m_cfgItem->group_id;
    return 0;
}

bool ItemBuff::init(int32_t itemid, int32_t time)
{
    m_cfgItem = CfgData::instance()->getItem(itemid);
    if (time)
        m_endTick = m_unit->getTick() + 1000 * time;
    else
        m_endTick = m_unit->getTick() + m_cfgBuff->duration;
    return m_cfgItem != nullptr;
}

void ItemBuff::extend(Buff *pBuff)
{
    if (pBuff)
    {
        m_endTick = m_endTick + pBuff->getCfgBuff()->duration;
    }
}

int32_t ItemBuff::getEndTime()
{
    int64_t m_endTick = this->m_endTick;
    int32_t Tick = m_unit->getTick();
    return roundInt((double)((int)m_endTick - Tick) * 0.001);
}

int32_t ItemBuff::getItemId()
{
    if (m_cfgItem)
        return m_cfgItem->id;
    return 0;
}
