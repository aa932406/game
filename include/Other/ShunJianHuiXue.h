#ifndef _SHUNJIANHUIXUE_H_
#define _SHUNJIANHUIXUE_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class ShunJianHuiXue : public ItemEffect
{
public:
    ShunJianHuiXue();
    virtual ~ShunJianHuiXue();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_addon;
};

#endif
