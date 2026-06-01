#ifndef _USEITEMGIFT_H_
#define _USEITEMGIFT_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"
#include "Other/ItemData.h"

class Player;
class Unit;

class UseItemGift : public ItemEffect
{
public:
    UseItemGift();
    virtual ~UseItemGift();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count) override;

private:
    int32_t nId;
    ItemData Item;
};

#endif
