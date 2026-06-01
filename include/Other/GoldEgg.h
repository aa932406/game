#ifndef _GOLDEGG_H_
#define _GOLDEGG_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;
class CfgGoldEgg;

class GoldEgg : public ItemEffect
{
public:
    GoldEgg();
    virtual ~GoldEgg();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count) override;

private:
    CfgGoldEgg* m_pCfgItem;
};

#endif
