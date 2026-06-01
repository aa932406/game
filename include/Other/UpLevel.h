#ifndef _UPLEVEL_H_
#define _UPLEVEL_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class UpLevel : public ItemEffect
{
public:
    UpLevel();
    virtual ~UpLevel();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count);

private:
    int32_t m_AddLevel;
    int32_t m_MaxLevel;
    int32_t m_MinLevel;
};

#endif
