#ifndef _RANDOMBUFF_H_
#define _RANDOMBUFF_H_

#include <cstdint>
#include <list>
#include <string>

#include "Other/ItemEffect.h"

class Player;
class Unit;
class Map;
class CActivityMap;
class CfgBuff;

class RandomBuff : public ItemEffect
{
public:
    struct randBuff
    {
        int32_t   nRate;       // 累计概率
        CfgBuff*  m_cfgBuff;   // buff配置
    };

    RandomBuff();
    ~RandomBuff();

    int32_t effect(Player* launcher, Unit* target, int32_t* count);
    bool parseEffect(int32_t id, const std::string* strEffect);

private:
    std::list<randBuff> m_buffs;
    int32_t             m_nSumRate = 0;
};

#endif // _RANDOMBUFF_H_
