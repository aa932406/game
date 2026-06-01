#ifndef _GONGMINGZHI_H_
#define _GONGMINGZHI_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class GongMingZhi : public ItemEffect
{
public:
    GongMingZhi();
    virtual ~GongMingZhi();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
    int32_t m_Chip;
};

#endif
