#include "Other/BossInfo.h"

#include <cstring>
#include <algorithm>

BossInfo::BossInfo()
    : m_nBossId(0)
{
    m_vRevivePos.clear();
}

BossInfo::BossInfo(const BossInfo& other)
    : m_nBossId(other.m_nBossId)
    , m_vRevivePos(other.m_vRevivePos)
{
}
