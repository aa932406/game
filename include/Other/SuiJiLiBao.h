#ifndef _SUIJILIBAO_H_
#define _SUIJILIBAO_H_

#include <cstdint>
#include <string>
#include <vector>
#include "Other/ItemEffect.h"

class Player;
struct CfgItemGiftRandom;

class SuiJiLiBao : public ItemEffect
{
public:
    SuiJiLiBao();
    virtual ~SuiJiLiBao();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    void broadcast(Player* launcher, int8_t nClass, int32_t nId);

private:
    std::vector<CfgItemGiftRandom> m_gifts;
    int32_t m_nId;
};

#endif
