#ifndef _SHOUCHONGITEM_H_
#define _SHOUCHONGITEM_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class ShouChongItem : public ItemEffect
{
public:
    ShouChongItem();
    virtual ~ShouChongItem();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t Value;
};

#endif
