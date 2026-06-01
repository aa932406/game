#ifndef _JIUSHUCARD_H_
#define _JIUSHUCARD_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class JiuShuCard : public ItemEffect
{
public:
    JiuShuCard();
    virtual ~JiuShuCard();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count) override;

private:
    int32_t m_addon;
};

#endif
