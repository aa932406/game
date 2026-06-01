#ifndef _FABAORES_H_
#define _FABAORES_H_

#include <cstdint>
#include <string>
#include <vector>
#include "Other/ItemEffect.h"

class FaBaoRes : public ItemEffect
{
public:
    FaBaoRes();
    virtual ~FaBaoRes();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_Type;
    std::vector<int32_t> m_Values;
};

#endif
