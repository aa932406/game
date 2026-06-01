#include "Database/WorshipDBData.h"

WorshipDBData::WorshipDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4F70; */
    std::string::string(&this->strCharList);
    /* base init: WorshipDBData::CleanUp(this); */
}

WorshipDBData::~WorshipDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D4F70; */
    std::string::~string(&this->strCharList);
}

void WorshipDBData::CleanUp()
{
    this->nTimes = 0;
    std::string::operator=(&this->strCharList, byte_8CFE00);
}

void WorshipDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    const char *v4; // rbx
    int32_t nTimes; // r12d
    const char *v6; // rax
    std::string __x; // [rsp+30h] [rbp-20h] BYREF
    _BYTE v10[17]; // [rsp+3Fh] [rbp-11h] BYREF

    bzero(szSQL, 0x1000u);
    v4 = (const char *)std::string::c_str(&this->strCharList);
    nTimes = this->nTimes;
    v6 = (const char *)std::string::c_str(&this->strCharList);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_char_worship` (cid,times,charlist) VALUES (%lld,%d,'%s') on duplicate key update times=%d,charlist='%s'",
    nCid,
    this->nTimes,
    v6,
    nTimes,
    v4);
    std::allocator<char>::allocator(v10);
    std::string::string(&__x, (char *)szSQL, v10);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(v10);
}

bool WorshipDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax

    Answer::MySqlQuery result; // [rsp+30h] [rbp-30h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_worship` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    if ( !Answer::MySqlQuery::eof(&result) )
    {
    this->nTimes = Answer::MySqlQuery::getIntValue(&result, "times", 0);
    StringValue = Answer::MySqlQuery::getStringValue(&result, "charlist", byte_8CFE00);
    std::string::operator=(&this->strCharList, StringValue);
    v8 = 1;
    }
    else
    {
    v8 = 0;
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return v8;
}

void WorshipDBData::PackageData(Answer::NetPacket *packet)
{
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, this->nTimes);
    Answer::NetPacket::writeUTF8(packet, &this->strCharList);
    }
}

（内容由AI生成，仅供参考）
