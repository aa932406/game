#include "Other/TeamMember.h"

#include <cstring>
#include <algorithm>

TeamMember::TeamMember()
    : nCharId(0)
    , nIndex(0)
{
}

TeamMember::TeamMember(CharId_t cid, const std::string& name, int16_t idx)
    : nCharId(cid)
    , strName(name)
    , nIndex(idx)
{
}

TeamMember::TeamMember(const TeamMember& other)
    : nCharId(other.nCharId)
    , strName(other.strName)
    , nIndex(other.nIndex)
{
}
