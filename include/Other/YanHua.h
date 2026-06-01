#ifndef _YANHUA_H_
#define _YANHUA_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Player;
class Unit;

class YanHua : public ItemEffect
{
public:
    YanHua();
    virtual ~YanHua();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

    int32_t effect(Player* launcher, Unit* target, int32_t* count) override;

private:
    int32_t nId;
    int32_t YanHuaType;
    int32_t YanHuaValue;
};

#endif
