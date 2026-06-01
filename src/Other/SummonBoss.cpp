#include "Other/SummonBoss.h"
#include "Common/Common.h"

SummonBoss::SummonBoss() : m_pCfgMapMonster(0), m_pCfgMonster(0), m_Count(0) {}
SummonBoss::~SummonBoss() {}
bool SummonBoss::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 2)
        return false;
    m_pCfgMapMonster = atoi(sv[0].c_str());
    m_pCfgMonster = atoi(sv[1].c_str());
    if (sv.size() >= 3) m_Count = atoi(sv[2].c_str());
    return m_pCfgMapMonster > 0;
}
