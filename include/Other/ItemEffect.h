#ifndef _ITEMEFFECT_H_
#define _ITEMEFFECT_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Unit;
class ItemEffect
{
public:
    virtual ~ItemEffect();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) = 0;
};

#endif
