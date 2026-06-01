#include "Utility/UnitHandle.h"

UnitHandle::UnitHandle()
    : id(0), type(static_cast<EntityType>(0))
{
}

UnitHandle::UnitHandle(EntityId_t id_, EntityType type_)
    : id(id_), type(type_)
{
}

bool UnitHandle::isEmpty() const
{
    return id == 0 || type == static_cast<EntityType>(0);
}
