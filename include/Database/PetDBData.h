#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class PetDBData : public IDataStruct {
public:
    PetDBData();
    ~PetDBData();
    void CleanUp(PetDBData *const this);
    void SaveToSqlString(PetDBData *const this, SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(PetDBData *const this, Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(PetDBData *const this, Answer::NetPacket *packet);
    void UnPackageData(PetDBData *const this, Answer::NetPacket *inPacket, CharId_t nCid);
    void paraseEquipString(PetDBData *const this, int32_t (*const vEquip)[4], const std::string *const str);
    void paraseSkillString(PetDBData *const this, int32_t (*const vSkill)[9], const std::string *const str);
    std::string paraseEquipString(PetDBData *const this, int32_t (*const vEquip)[4]);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
