#ifndef _MINGGECHIP_H_
#define _MINGGECHIP_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class MingGeChip : public ItemEffect
{
public:
    MingGeChip();
    virtual ~MingGeChip();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
    int32_t m_Chip;
};

#endif
