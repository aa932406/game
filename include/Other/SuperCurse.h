#ifndef _SUPERCURSE_H_
#define _SUPERCURSE_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class SuperCurse : public ItemEffect
{
public:
    SuperCurse();
    virtual ~SuperCurse();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t Value;
};

#endif
