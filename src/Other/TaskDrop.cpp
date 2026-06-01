#include "Other/TaskDrop.h"

#include <cstring>
#include <algorithm>

TaskDrop::TaskDrop(const TaskDrop *const a2)
{
    this->nCount = a2->nCount;
    std::list<Param2>::list(&this->lstJobGroup, &a2->lstJobGroup);
}

