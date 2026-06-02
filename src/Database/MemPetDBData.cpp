#include "Database/MemPetDBData.h"

MemPetDBData::MemPetDBData()
    : nOwner(0)
    , nPetId(0)
    , nBaseId(0)
{
}

MemPetDBData::~MemPetDBData()
{
}

void MemPetDBData::CleanUp()
{
    strName.clear();
    skills.clear();
    equips.clear();
    nOwner = 0;
    nPetId = 0;
    nBaseId = 0;
}
