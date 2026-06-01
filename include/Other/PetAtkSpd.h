#ifndef _PETATKSPD_H_
#define _PETATKSPD_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class PetAtkSpd : public ItemEffect
{
public:
    PetAtkSpd();
    virtual ~PetAtkSpd();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t Value;
};

#endif
