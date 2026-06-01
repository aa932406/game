#ifndef _SUPEREXCHANGE_H_
#define _SUPEREXCHANGE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class SuperExchange : public ItemEffect
{
public:
    struct SuperExchangeToItem
    {
        int32_t type;
        int32_t value;
    };

    SuperExchange();
    virtual ~SuperExchange();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count) override;

private:
    int32_t m_id;
    std::vector<SuperExchangeToItem> m_exchangeToItems;
};

#endif
