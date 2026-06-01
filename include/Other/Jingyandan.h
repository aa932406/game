#ifndef _JINGYANDAN_H_
#define _JINGYANDAN_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class Jingyandan : public ItemEffect
{
public:
    Jingyandan();
    virtual ~Jingyandan();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_exp;
};

#endif
