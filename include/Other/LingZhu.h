#ifndef _LINGZHU_H_
#define _LINGZHU_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class LingZhu : public ItemEffect
{
public:
    LingZhu();
    virtual ~LingZhu();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count);

private:
    int32_t m_nId;
    int32_t m_Chip;
};

#endif
