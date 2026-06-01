#include "Database/PetDBData.h"

PetDBData::PetDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4FB0; */
    /* base init: PetDBData::CleanUp(this); */
}

PetDBData::~PetDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4FB0; */
}

void PetDBData::CleanUp()
{
    bzero(&this->petData, 0x78u);
}

void PetDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    int32_t nHuanHua; // r14d
    const char *v5; // r12
    const char *v6; // r13
    const char *v7; // rax
    int AiState; // [rsp+B4h] [rbp-BCh]
    int32_t nDieTime; // [rsp+B8h] [rbp-B8h]
    int32_t nStarExp; // [rsp+BCh] [rbp-B4h]
    int32_t nStar; // [rsp+C0h] [rbp-B0h]
    int nState; // [rsp+C4h] [rbp-ACh]
    int bAlive; // [rsp+C8h] [rbp-A8h]
    int32_t nHP; // [rsp+CCh] [rbp-A4h]
    PetId_t nPetId; // [rsp+D8h] [rbp-98h]


    const char *v18; // [rsp+E8h] [rbp-88h]
    std::string skill; // [rsp+110h] [rbp-60h] BYREF
    std::string equip; // [rsp+120h] [rbp-50h] BYREF
    std::string __x; // [rsp+130h] [rbp-40h] BYREF
    _BYTE v24[49]; // [rsp+13Fh] [rbp-31h] BYREF

    PetDBData::paraseEquipString((PetDBData *const)&equip, (int32_t (*const)[4])this);
    PetDBData::paraseSkillString((PetDBData *const)&skill, (int32_t (*const)[9])this);
    bzero(szSQL, 0x1000u);
    nHuanHua = this->petData.nHuanHua;
    AiState = this->petData.AiState;
    v5 = (const char *)std::string::c_str(&skill);
    v6 = (const char *)std::string::c_str(&equip);
    nDieTime = this->petData.nDieTime;
    nStarExp = this->petData.nStarExp;
    nStar = this->petData.nStar;
    nState = this->petData.nState;
    bAlive = this->petData.bAlive;
    nHP = this->petData.nHP;
    nPetId = this->petData.nPetId;
    v16 = this->petData.nHuanHua;
    v17 = this->petData.AiState;
    v18 = (const char *)std::string::c_str(&skill);
    v7 = (const char *)std::string::c_str(&equip);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_chr_pet`( `cid`,`pid`,`name`,`hp`,`alive`,`state`,`star`,`starExp`,`dieTime`,`equip`,`skill`,`AiSta"
    "te`,`HuanHua` ) VALUES (%lld,%lld,'%s',%d,%d,%d,%d,%d,%d,'%s','%s',%d,%d) ON DUPLICATE KEY UPDATE `pid`=%lld,`name`="
    "'%s',`hp`=%d,`alive`=%d,`state`=%d,`star`=%d, `starExp`=%d,`dieTime`=%d,`equip`='%s',`skill`='%s',`AiState`=%d,`HuanHua`=%d",
    nCid,
    this->petData.nPetId,
    this->petData.strName,
    this->petData.nHP,
    this->petData.bAlive,
    this->petData.nState,
    this->petData.nStar,
    this->petData.nStarExp,
    this->petData.nDieTime,
    v7,
    v18,
    v17,
    v16,
    nPetId,
    this->petData.strName,
    nHP,
    bAlive,
    nState,
    nStar,
    nStarExp,
    nDieTime,
    v6,
    v5,
    AiState,
    nHuanHua);
    std::allocator<char>::allocator(v24);
    std::string::string(&__x, (char *)szSQL, v24);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(v24);
    std::string::~string(&skill);
    std::string::~string(&equip);
}

bool PetDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax
    const char *v8; // rax
    const char *v9; // rax
    Answer::MySqlQuery result; // [rsp+30h] [rbp-50h] BYREF
    std::string str; // [rsp+50h] [rbp-30h] BYREF


    _BYTE v17[17]; // [rsp+6Fh] [rbp-11h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_chr_pet` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    if ( !Answer::MySqlQuery::eof(&result) )
    {
    bzero(&this->petData, 0x78u);
    this->petData.nPetId = Answer::MySqlQuery::getInt64Value(&result, "pid", 0);
    StringValue = Answer::MySqlQuery::getStringValue(&result, "name", byte_8CFE00);
    snprintf(this->petData.strName, 0x1Eu, StringValue);
    this->petData.nHP = Answer::MySqlQuery::getIntValue(&result, "hp", 0);
    this->petData.bAlive = Answer::MySqlQuery::getIntValue(&result, "alive", 0);
    this->petData.nState = Answer::MySqlQuery::getIntValue(&result, "state", 0);
    this->petData.nStar = Answer::MySqlQuery::getIntValue(&result, "star", 0);
    this->petData.nStarExp = Answer::MySqlQuery::getIntValue(&result, "starExp", 0);
    this->petData.nDieTime = Answer::MySqlQuery::getIntValue(&result, "dieTime", 0);
    this->petData.AiState = Answer::MySqlQuery::getIntValue(&result, "AiState", 0);
    this->petData.nHuanHua = Answer::MySqlQuery::getIntValue(&result, "HuanHua", 0);

    v8 = Answer::MySqlQuery::getStringValue(&result, "equip", byte_8CFE00);
    std::string::string(&str, v8, &v15);
    this->paraseEquipString((int32_t (*const)[4])this->petData.vEquip, &str);
    std::string::~string(&str);
    std::allocator<char>::~allocator(&v15);
    std::allocator<char>::allocator(v17);
    v9 = Answer::MySqlQuery::getStringValue(&result, "skill", byte_8CFE00);
    std::string::string(&v16, v9, v17);
    this->paraseSkillString((int32_t (*const)[9])this->petData.vSkillLvel, &v16);
    std::string::~string(&v16);
    std::allocator<char>::~allocator(v17);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return 1;
}

void PetDBData::PackageData(Answer::NetPacket *packet)
{
    std::string val; // [rsp+10h] [rbp-20h] BYREF

    int8_t i; // [rsp+1Eh] [rbp-12h]
    int8_t i_0; // [rsp+1Fh] [rbp-11h]

    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, this->petData.nPetId);

    std::string::string(&val, this->petData.strName, &v3);
    Answer::NetPacket::writeUTF8(packet, &val);
    std::string::~string(&val);
    std::allocator<char>::~allocator(&v3);
    Answer::NetPacket::writeInt32(packet, this->petData.nHP);
    Answer::NetPacket::writeInt8(packet, this->petData.bAlive);
    Answer::NetPacket::writeInt8(packet, this->petData.nState);
    Answer::NetPacket::writeInt32(packet, this->petData.nStar);
    Answer::NetPacket::writeInt32(packet, this->petData.nStarExp);
    Answer::NetPacket::writeInt32(packet, this->petData.nDieTime);
    Answer::NetPacket::writeInt8(packet, this->petData.AiState);
    Answer::NetPacket::writeInt32(packet, this->petData.nHuanHua);
    for ( i = 0; i <= 3; ++i )
    Answer::NetPacket::writeInt32(packet, this->petData.vEquip[i]);
    for ( i_0 = 0; i_0 <= 8; ++i_0 )
    Answer::NetPacket::writeInt32(packet, this->petData.vSkillLvel[i_0]);
    }
}

void PetDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{
    const char *v3; // rax


    int8_t i; // [rsp+2Eh] [rbp-12h]
    int8_t i_0; // [rsp+2Fh] [rbp-11h]

    if ( inPacket )
    {
    this->petData.nPetId = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&v6, (bool)inPacket);
    v3 = (const char *)std::string::c_str(&v6);
    snprintf(this->petData.strName, 0x1Eu, v3);
    std::string::~string(&v6);
    this->petData.nHP = Answer::NetPacket::readInt32(inPacket);
    this->petData.bAlive = Answer::NetPacket::readInt8(inPacket);
    this->petData.nState = Answer::NetPacket::readInt8(inPacket);
    this->petData.nStar = Answer::NetPacket::readInt32(inPacket);
    this->petData.nStarExp = Answer::NetPacket::readInt32(inPacket);
    this->petData.nDieTime = Answer::NetPacket::readInt32(inPacket);
    this->petData.AiState = Answer::NetPacket::readInt8(inPacket);
    this->petData.nHuanHua = Answer::NetPacket::readInt32(inPacket);
    for ( i = 0; i <= 3; ++i )
    {
    v4 = i;
    this->petData.vEquip[v4] = Answer::NetPacket::readInt32(inPacket);
    }
    for ( i_0 = 0; i_0 <= 8; ++i_0 )
    {
    v5 = i_0;
    this->petData.vSkillLvel[v5] = Answer::NetPacket::readInt32(inPacket);
    }
    }
}

void PetDBData::paraseEquipString(int32_t (*const vEquip)[4], const std::string *const str)
{


    const char *v5; // rax

    StringVector vStr; // [rsp+20h] [rbp-40h] BYREF
    std::string delims; // [rsp+40h] [rbp-20h] BYREF


    if ( !std::operator==<char>(str, byte_8CFE00) )
    {

    std::string::string(&delims, ":", &v11);
    Answer::StringUtility::split(&vStr, str, &delims, 0);
    std::string::~string(&delims);
    std::allocator<char>::~allocator(&v11);
    if ( !vStr.empty() )
    {
    for ( i = 0; ; ++i )
    {
    v6 = i;
    if ( v6 >= vStr.size() || i > 3 )
    break;
    v3 = i;
    v4 = std::vector<std::string>::operator[](&vStr, i);
    v5 = (const char *)std::string::c_str(v4);
    (*vEquip)[v3] = atoi(v5);
    }
    }
    std::vector<std::string>::~vector(&vStr);
    }
}

void PetDBData::paraseSkillString(int32_t (*const vSkill)[9], const std::string *const str)
{


    const char *v5; // rax

    StringVector vStr; // [rsp+20h] [rbp-40h] BYREF
    std::string delims; // [rsp+40h] [rbp-20h] BYREF


    if ( !std::operator==<char>(str, byte_8CFE00) )
    {

    std::string::string(&delims, ":", &v11);
    Answer::StringUtility::split(&vStr, str, &delims, 0);
    std::string::~string(&delims);
    std::allocator<char>::~allocator(&v11);
    if ( !vStr.empty() )
    {
    for ( i = 0; ; ++i )
    {
    v6 = i;
    if ( v6 >= vStr.size() || i > 8 )
    break;
    v3 = i;
    v4 = std::vector<std::string>::operator[](&vStr, i);
    v5 = (const char *)std::string::c_str(v4);
    (*vSkill)[v3] = atoi(v5);
    }
    }
    std::vector<std::string>::~vector(&vStr);
    }
}

std::string PetDBData::paraseEquipString(int32_t (*const vEquip)[4])
{


    int32_t *vEquipa; // [rsp+0h] [rbp-1A0h]
    _BYTE v6[16]; // [rsp+10h] [rbp-190h] BYREF
    _BYTE v7[364]; // [rsp+20h] [rbp-180h] BYREF


    vEquipa = v2;
    v3 = std::operator|(std::_Ios_Openmode::_S_out, std::_Ios_Openmode::_S_in);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::basic_stringstream(v6, (unsigned int)v3);
    for ( i = 0; i <= 3; ++i )
    {
    if ( i )
    std::operator<<<std::char_traits<char>>(v7, ":");
    std::ostream::operator<<(v7, (unsigned int)vEquipa[i]);
    }
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::str(this, v6);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::~basic_stringstream(v6);
    return (std::string)this;
}

（内容由AI生成，仅供参考）
