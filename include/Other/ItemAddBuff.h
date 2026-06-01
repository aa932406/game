#ifndef _ITEMADDBUFF_H_
#define _ITEMADDBUFF_H_

#include <cstdint>
#include <string>
#include <list>
#include "Other/ItemEffect.h"

class CfgBuff;

class ItemAddBuff : public ItemEffect
{
public:
    ItemAddBuff();
    virtual ~ItemAddBuff();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_nItemId;
    std::list<CfgBuff*> m_lBuffs;
};

#endif
