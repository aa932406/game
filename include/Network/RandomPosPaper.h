#ifndef _RANDOMPOSPAPER_H_
#define _RANDOMPOSPAPER_H_

#include <cstdint>
#include <string>

#include "Other/ItemEffect.h"

class Player;
class Unit;

class RandomPosPaper : public ItemEffect
{
public:
    RandomPosPaper();
    ~RandomPosPaper();

    int32_t effect(Player* launcher, Unit* target, int32_t* count);
    bool parseEffect(int32_t id, const std::string* strEffect);

private:
    int32_t m_itemid = 0;
};

#endif // _RANDOMPOSPAPER_H_
