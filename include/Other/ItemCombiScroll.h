#ifndef _ITEMCOMBISCROLL_H_
#define _ITEMCOMBISCROLL_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class ItemCombiScroll : public ItemEffect
{
public:
    ItemCombiScroll();
    virtual ~ItemCombiScroll();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
};

#endif
