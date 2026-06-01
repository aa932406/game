#ifndef _PETSHIHUA_H_
#define _PETSHIHUA_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class PetShiHua : public ItemEffect
{
public:
    PetShiHua();
    virtual ~PetShiHua();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t Value;
};

#endif
