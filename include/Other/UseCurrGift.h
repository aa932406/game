#ifndef _USECURRGIFT_H_
#define _USECURRGIFT_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class UseCurrGift : public ItemEffect
{
public:
    UseCurrGift();
    virtual ~UseCurrGift();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t nId;
    int32_t CurrType;
    int32_t CurrValue;
};

#endif
