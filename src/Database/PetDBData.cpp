#include "Database/PetDBData.h"
#include <sstream>
#include "Answer/MySqlQuery.h"
#include "Answer/MySqlDBGuard.h"
#include "Answer/NetPacket.h"
#include "Answer/StringUtility.h"

PetDBData::PetDBData()
    : nPetId(0)
    , nHP(0)
    , bAlive(0)
    , nState(0)
    , nStar(0)
    , nStarExp(0)
    , nDieTime(0)
    , AiState(0)
    , nHuanHua(0)
{
    bzero(strName, sizeof(strName));
    bzero(vEquip, sizeof(vEquip));
    bzero(vSkillLvel, sizeof(vSkillLvel));
}

PetDBData::~PetDBData()
{
}

void PetDBData::CleanUp()
{
    nPetId = 0;
    bzero(strName, sizeof(strName));
    nHP = 0;
    bAlive = 0;
    nState = 0;
    nStar = 0;
    nStarExp = 0;
    nDieTime = 0;
    bzero(vEquip, sizeof(vEquip));
    bzero(vSkillLvel, sizeof(vSkillLvel));
    AiState = 0;
    nHuanHua = 0;
}

void PetDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    std::string equipStr = paraseEquipString(&vEquip);
    std::string skillStr;
    {
        // Serialize skills
        std::stringstream ss;
        for (int i = 0; i < 9; ++i)
        {
            if (i > 0) ss << ":";
            ss << vSkillLvel[i];
        }
        skillStr = ss.str();
    }

    bzero(szSQL, 0x1000u);
    snprintf(
        (char *)szSQL,
        0xFFFu,
        "INSERT INTO `mem_chr_pet`(`cid`,`pid`,`name`,`hp`,`alive`,`state`,`star`,`starExp`,`dieTime`,`equip`,`skill`,`AiState`,`HuanHua`) "
        "VALUES (%lld,%lld,'%s',%d,%d,%d,%d,%d,%d,'%s','%s',%d,%d) "
        "ON DUPLICATE KEY UPDATE `pid`=%lld,`name`='%s',`hp`=%d,`alive`=%d,`state`=%d,`star`=%d,`starExp`=%d,`dieTime`=%d,`equip`='%s',`skill`='%s',`AiState`=%d,`HuanHua`=%d",
        nCid,
        nPetId,
        strName,
        nHP,
        bAlive,
        nState,
        nStar,
        nStarExp,
        nDieTime,
        equipStr.c_str(),
        skillStr.c_str(),
        AiState,
        nHuanHua,
        nPetId,
        strName,
        nHP,
        bAlive,
        nState,
        nStar,
        nStarExp,
        nDieTime,
        equipStr.c_str(),
        skillStr.c_str(),
        AiState,
        nHuanHua);

    sqls->push_back(std::string((char*)szSQL));
}

bool PetDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{
    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_chr_pet` WHERE `cid`=%lld", nCid);

    Answer::MySqlQuery result(db->query((const char *)szSQL));
    if (!result.eof())
    {
        nPetId = 0;
        bzero(strName, sizeof(strName));
        nHP = 0;
        bAlive = 0;
        nState = 0;
        nStar = 0;
        nStarExp = 0;
        nDieTime = 0;
        bzero(vEquip, sizeof(vEquip));
        bzero(vSkillLvel, sizeof(vSkillLvel));
        AiState = 0;
        nHuanHua = 0;

        nPetId = result.getInt64Value("pid", 0);
        std::string name = result.getStringValue("name", "");
        snprintf(strName, sizeof(strName), "%s", name.c_str());
        nHP = result.getIntValue("hp", 0);
        bAlive = (int8_t)result.getIntValue("alive", 0);
        nState = (int8_t)result.getIntValue("state", 0);
        nStar = result.getIntValue("star", 0);
        nStarExp = result.getIntValue("starExp", 0);
        nDieTime = result.getIntValue("dieTime", 0);
        AiState = (int8_t)result.getIntValue("AiState", 0);
        nHuanHua = result.getIntValue("HuanHua", 0);

        std::string equipStr = result.getStringValue("equip", "");
        paraseEquipString(&vEquip, &equipStr);

        std::string skillStr = result.getStringValue("skill", "");
        paraseSkillString(&vSkillLvel, &skillStr);
    }
    return true;
}

void PetDBData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;

    Answer::NetPacket::writeInt64(packet, nPetId);
    {
        std::string name(strName);
        Answer::NetPacket::writeUTF8(packet, &name);
    }
    Answer::NetPacket::writeInt32(packet, nHP);
    Answer::NetPacket::writeInt8(packet, bAlive);
    Answer::NetPacket::writeInt8(packet, nState);
    Answer::NetPacket::writeInt32(packet, nStar);
    Answer::NetPacket::writeInt32(packet, nStarExp);
    Answer::NetPacket::writeInt32(packet, nDieTime);
    Answer::NetPacket::writeInt8(packet, AiState);
    Answer::NetPacket::writeInt32(packet, nHuanHua);
    for (int i = 0; i < 4; ++i)
        Answer::NetPacket::writeInt32(packet, vEquip[i]);
    for (int i = 0; i < 9; ++i)
        Answer::NetPacket::writeInt32(packet, vSkillLvel[i]);
}

void PetDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{
    if (!inPacket) return;

    nPetId = Answer::NetPacket::readInt64(inPacket);
    std::string name;
    Answer::NetPacket::readUTF8(inPacket, &name);
    snprintf(strName, sizeof(strName), "%s", name.c_str());
    nHP = Answer::NetPacket::readInt32(inPacket);
    bAlive = Answer::NetPacket::readInt8(inPacket);
    nState = Answer::NetPacket::readInt8(inPacket);
    nStar = Answer::NetPacket::readInt32(inPacket);
    nStarExp = Answer::NetPacket::readInt32(inPacket);
    nDieTime = Answer::NetPacket::readInt32(inPacket);
    AiState = Answer::NetPacket::readInt8(inPacket);
    nHuanHua = Answer::NetPacket::readInt32(inPacket);
    for (int i = 0; i < 4; ++i)
        vEquip[i] = Answer::NetPacket::readInt32(inPacket);
    for (int i = 0; i < 9; ++i)
        vSkillLvel[i] = Answer::NetPacket::readInt32(inPacket);
}

void PetDBData::paraseEquipString(int32_t (*const vEquip)[4], const std::string *const str)
{
    if (!str || str->empty()) return;

    std::string delims = ":";
    StringVector vStr;
    StringUtility::split(vStr, *str, delims);
    for (size_t i = 0; i < vStr.size() && i < 4; ++i)
    {
        (*vEquip)[i] = atoi(vStr[i].c_str());
    }
}

void PetDBData::paraseSkillString(int32_t (*const vSkill)[9], const std::string *const str)
{
    if (!str || str->empty()) return;

    std::string delims = ":";
    StringVector vStr;
    StringUtility::split(vStr, *str, delims);
    for (size_t i = 0; i < vStr.size() && i < 9; ++i)
    {
        (*vSkill)[i] = atoi(vStr[i].c_str());
    }
}

std::string PetDBData::paraseEquipString(int32_t (*const vEquip)[4])
{
    std::stringstream ss;
    for (int i = 0; i < 4; ++i)
    {
        if (i > 0) ss << ":";
        ss << (*vEquip)[i];
    }
    return ss.str();
}
