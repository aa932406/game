#include "Other/Entity.h"

#include <cstring>
#include <algorithm>
#include <atomic>

int64_t Entity::m_nextEntityId = 0;

Entity::Entity(EntityType type)
    : m_entityId(0)
    , m_type(type)
{
    generateEntityId();
}

Entity::~Entity()
{
}

void Entity::generateEntityId()
{
    m_entityId = InterlockedExchangeAdd64(&m_nextEntityId, 1) + 0x100000000;
}