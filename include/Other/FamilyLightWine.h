#ifndef _FAMILYLIGHTWINE_H_
#define _FAMILYLIGHTWINE_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class FamilyLightWine : public ItemEffect
{
public:
    FamilyLightWine();
    virtual ~FamilyLightWine();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_itemid;
    int32_t m_cfgBuff[6];
};

#endif
