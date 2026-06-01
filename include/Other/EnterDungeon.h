#ifndef _ENTERDUNGEON_H_
#define _ENTERDUNGEON_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class EnterDungeon : public ItemEffect
{
public:
    EnterDungeon();
    virtual ~EnterDungeon();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count) override;

private:
    int32_t m_DungeonId;
};

#endif
