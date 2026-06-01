#ifndef _SKILLBOOK_H_
#define _SKILLBOOK_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class SkillBook : public ItemEffect
{
public:
    SkillBook();
    virtual ~SkillBook();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_SkillId;
    int32_t m_itemid;
};

#endif
