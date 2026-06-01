#ifndef _MINGGEEXP_H_
#define _MINGGEEXP_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class MingGeExp : public ItemEffect
{
public:
    MingGeExp();
    virtual ~MingGeExp();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
    int32_t m_Chip;
};

#endif
