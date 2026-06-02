#include "Other/TaskDrop.h"

#include <cstring>
#include <algorithm>

TaskDrop::TaskDrop(const TaskDrop& a2)
{
    this->nCount = a2.nCount;
    this->lstJobGroup = a2.lstJobGroup;
}

