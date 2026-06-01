#ifndef _WINGLEVELUP_H_
#define _WINGLEVELUP_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class WingLevelUp : public ItemEffect
{
public:
    WingLevelUp();
    virtual ~WingLevelUp();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t UseWingLevel;
    int32_t UpLevel;
    int32_t ItemId;
    int32_t ItemCount;
};

#endif
