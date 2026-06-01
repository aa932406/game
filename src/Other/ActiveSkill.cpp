#include "Other/ActiveSkill.h"

ActiveSkill::ActiveSkill()
    : nSkillId(0)
    , nEntityId(0)
    , nType(0)
{
}

ActiveSkill::ActiveSkill(int32_t skillId, EntityId_t entityId, int32_t type)
    : nSkillId(skillId)
    , nEntityId(entityId)
    , nType(type)
{
}
