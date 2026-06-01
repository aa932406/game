#include "Database/MagicBoxDBData.h"

MagicBoxDBData::MagicBoxDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D5470; */
    std::string::string(&this->strActiveScrolls);
    std::string::string(&this->strSuccessIds);
    /* base init: MagicBoxDBData::CleanUp(this); */
}

MagicBoxDBData::~MagicBoxDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D5470; */
    std::string::~string(&this->strSuccessIds);
    std::string::~string(&this->strActiveScrolls);
}

void MagicBoxDBData::CleanUp()
{
    this->nCombiPoints = 0;
    this->nLastReviveTime = 0;
    std::string::operator=(&this->strActiveScrolls, byte_8CFE00);
    std::string::operator=(&this->strSuccessIds, byte_8CFE00);
}

void MagicBoxDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    const char *v4; // r13
    const char *v5; // r12
    int32_t nLastReviveTime; // r15d
    int32_t nCombiPoints; // r14d
    const char *v8; // rbx
    const char *v9; // rax
    std::string __x; // [rsp+50h] [rbp-40h] BYREF
    _BYTE v13[49]; // [rsp+5Fh] [rbp-31h] BYREF

    bzero(szSQL, 0x1000u);
    v4 = (const char *)std::string::c_str(&this->strSuccessIds);
    v5 = (const char *)std::string::c_str(&this->strActiveScrolls);
    nLastReviveTime = this->nLastReviveTime;
    nCombiPoints = this->nCombiPoints;
    v8 = (const char *)std::string::c_str(&this->strSuccessIds);
    v9 = (const char *)std::string::c_str(&this->strActiveScrolls);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_char_magic_box` (`cid`,`combi_points`,`revive_time`,`active_scrolls`,`success_ids`) VALUES (%lld,%d"
    ",%d,'%s','%s') ON DUPLICATE KEY UPDATE `combi_points`=%d,`revive_time`=%d,`active_scrolls`='%s',`success_ids`='%s'",
    nCid,
    this->nCombiPoints,
    this->nLastReviveTime,
    v9,
    v8,
    nCombiPoints,
    nLastReviveTime,
    v5,
    v4);
    std::allocator<char>::allocator(v13);
    std::string::string(&__x, (char *)szSQL, v13);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(v13);
}

bool MagicBoxDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax
    const char *v8; // rax
    Answer::MySqlQuery result; // [rsp+30h] [rbp-30h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_magic_box` WHERE `cid`= %lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    if ( !Answer::MySqlQuery::eof(&result) )
    {
    this->nCombiPoints = Answer::MySqlQuery::getIntValue(&result, "combi_points", 0);
    this->nLastReviveTime = Answer::MySqlQuery::getIntValue(&result, "revive_time", 0);
    StringValue = Answer::MySqlQuery::getStringValue(&result, "active_scrolls", byte_8CFE00);
    std::string::operator=(&this->strActiveScrolls, StringValue);
    v8 = Answer::MySqlQuery::getStringValue(&result, "success_ids", byte_8CFE00);
    std::string::operator=(&this->strSuccessIds, v8);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return 1;
}

void MagicBoxDBData::PackageData(Answer::NetPacket *packet)
{
    Answer::NetPacket::writeInt32(packet, this->nCombiPoints);
    Answer::NetPacket::writeInt32(packet, this->nLastReviveTime);
    Answer::NetPacket::writeUTF8(packet, &this->strActiveScrolls);
    Answer::NetPacket::writeUTF8(packet, &this->strSuccessIds);
}

（内容由AI生成，仅供参考）
