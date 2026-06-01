#ifndef _TITLECARD_H_
#define _TITLECARD_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class TitleCard : public ItemEffect
{
public:
    TitleCard();
    virtual ~TitleCard();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_TitleId;
    int32_t m_Time;
};

#endif
