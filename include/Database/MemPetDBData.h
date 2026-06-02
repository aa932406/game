#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Database/PlayerDBData.h"

class MemPetDBData : public IDataStruct {
public:
    MemPetDBData();
    ~MemPetDBData();
    void CleanUp();

    std::string strName;
    std::string skills;
    std::string equips;
    int32_t nOwner;
    int32_t nPetId;
    int32_t nBaseId;
};
