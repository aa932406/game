#ifndef _SUMMONBOSS_H_
#define _SUMMONBOSS_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class SummonBoss : public ItemEffect
{
public:
    SummonBoss();
    virtual ~SummonBoss();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_pCfgMapMonster;
    int32_t m_pCfgMonster;
    int32_t m_Count;
};

#endif
