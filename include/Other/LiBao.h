#ifndef _LIBAO_H_
#define _LIBAO_H_

#include <cstdint>
#include <string>
#include <vector>
#include "Other/ItemEffect.h"

class Player;
class Unit;
struct CfgItemGift;

class LiBao : public ItemEffect
{
public:
    LiBao();
    virtual ~LiBao();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count);

private:
    std::vector<CfgItemGift> m_gifts;
    int32_t m_month;
    int32_t m_record;
};

#endif
