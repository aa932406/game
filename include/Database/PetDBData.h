#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Database/PlayerDBData.h"

class PetDBData : public IDataStruct {
public:
    PetDBData();
    ~PetDBData();
    void CleanUp();
    void SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid);
    void paraseEquipString(int32_t (*const vEquip)[4], const std::string *const str);
    void paraseSkillString(int32_t (*const vSkill)[9], const std::string *const str);
    std::string paraseEquipString(int32_t (*const vEquip)[4]);

    int64_t nPetId;
    char strName[30];
    int32_t nHP;
    int8_t bAlive;
    int8_t nState;
    int32_t nStar;
    int32_t nStarExp;
    int32_t nDieTime;
    int32_t vEquip[4];
    int32_t vSkillLvel[9];
    int8_t AiState;
    int32_t nHuanHua;
};
