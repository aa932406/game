#ifndef _RECHARGECARD_H_
#define _RECHARGECARD_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class RechargeCard : public ItemEffect
{
public:
    RechargeCard();
    virtual ~RechargeCard();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_addon;
};

#endif
