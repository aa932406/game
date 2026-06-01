#ifndef _MINGGEITEM_H_
#define _MINGGEITEM_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class MingGeItem : public ItemEffect
{
public:
    MingGeItem();
    virtual ~MingGeItem();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
    int32_t m_Chip;
};

#endif
