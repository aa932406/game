#ifndef _ITEMCOMBIPOINT_H_
#define _ITEMCOMBIPOINT_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class ItemCombiPoint : public ItemEffect
{
public:
    ItemCombiPoint();
    virtual ~ItemCombiPoint();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nValue;
};

#endif
