#include "Other/cfgSpecialMonsterTable.h"

#include <cstring>
#include <algorithm>

void cfgSpecialMonsterTable::Add(cfgSpecialMonster *const stu)
{
    *std::map<int,cfgSpecialMonster>::operator[](&this->m_specialMonster, &stu->m_mid) = *stu;
}

