#ifndef _UNITHANDLE_H_
#define _UNITHANDLE_H_

#include "Common/CommonTypes.h"
#include "Common/EntityType.h"

class UnitHandle
{
public:
    UnitHandle();
    UnitHandle(EntityId_t id, EntityType type);

    bool isEmpty() const;

    // 成员变量
    EntityId_t  id;
    EntityType  type;
};

#endif // _UNITHANDLE_H_