#ifndef _GOBLINITEM_H_
#define _GOBLINITEM_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class GoblinItem : public ItemEffect
{
public:
    GoblinItem();
    virtual ~GoblinItem();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_Type;
    int32_t m_MapId;
    int32_t m_X;
    int32_t m_Y;
    int32_t m_Range;
};

#endif
