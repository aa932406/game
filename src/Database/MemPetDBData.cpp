#include "Database/MemPetDBData.h"

MemPetDBData::MemPetDBData()
{
    std::string::string(&this->strName);
    std::string::string(&this->skills);
    std::string::string(&this->equips);
    /* base init: MemPetDBData::CleanUp(this); */
}

MemPetDBData::~MemPetDBData()
{
    std::string::~string(&this->equips);
    std::string::~string(&this->skills);
    std::string::~string(&this->strName);
}

void MemPetDBData::CleanUp()
{
    this->nOwner = 0;
    this->nPetId = 0;
    this->nBaseId = 0;
    std::string::operator=(&this->strName, &byte_8F560F);
    std::string::operator=(&this->skills, &byte_8F560F);
    std::string::operator=(&this->equips, &byte_8F560F);
}

（内容由AI生成，仅供参考）
