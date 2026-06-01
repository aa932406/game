#ifndef _VIPDROP_H_
#define _VIPDROP_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class VipDrop : public ItemEffect
{
public:
    VipDrop();
    virtual ~VipDrop();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count);

private:
    int32_t m_Type;
};

#endif
