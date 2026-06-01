#ifndef _MONEYGAIN_H_
#define _MONEYGAIN_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class MoneyGain : public ItemEffect
{
public:
    MoneyGain();
    virtual ~MoneyGain();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_CurrecyType;
    int32_t m_CurrecyCount;
};

#endif
