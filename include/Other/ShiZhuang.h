#ifndef _SHIZHUANG_H_
#define _SHIZHUANG_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class ShiZhuang : public ItemEffect
{
public:
    ShiZhuang();
    virtual ~ShiZhuang();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nId;
    int32_t m_nShiZhuangId;
    int32_t m_nTime;
};

#endif
