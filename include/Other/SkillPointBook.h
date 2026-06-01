#ifndef _SKILLPOINTBOOK_H_
#define _SKILLPOINTBOOK_H_

#include <cstdint>
#include <string>
#include "Other/ItemEffect.h"

class SkillPointBook : public ItemEffect
{
public:
    SkillPointBook();
    virtual ~SkillPointBook();
    virtual bool parseEffect(int32_t id, const std::string* strEffect) override;

private:
    int32_t m_SkillId;
    int32_t m_SkillPoints;
    int32_t m_itemid;
};

#endif
