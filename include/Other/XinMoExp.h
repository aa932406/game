#ifndef _XINMOEXP_H_
#define _XINMOEXP_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class XinMoExp : public ItemEffect
{
public:
    XinMoExp();
    virtual ~XinMoExp();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
    int32_t m_Exp;
};

#endif
