#include "Database/CharWingDBData.h"

CharWingDBData::CharWingDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D54B0; */
    /* base init: CharWingDBData::CleanUp(this); */
}

CharWingDBData::~CharWingDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D54B0; */
}

void CharWingDBData::CleanUp()
{
    this->m_Level = 0;
    this->m_Luck = 0;
    this->m_HuanHua = 0;
}

void CharWingDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    std::string __x; // [rsp+40h] [rbp-20h] BYREF
    _BYTE v7[17]; // [rsp+4Fh] [rbp-11h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "INSERT INTO `mem_char_wing` (`cid`,`Level`, `Luck`,`HuanHua`) VALUES (%lld,%d,%d,%d) ON DUPLICATE KEY UPDATE `Level`"
    " = %d,`Luck` = %d,`HuanHua`=%d",
    nCid,
    this->m_Level,
    this->m_Luck,
    this->m_HuanHua,
    this->m_Level,
    this->m_Luck,
    this->m_HuanHua);
    std::allocator<char>::allocator(v7);
    std::string::string(&__x, (char *)szSQL, v7);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(v7);
}

bool CharWingDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    Answer::MySqlQuery result; // [rsp+30h] [rbp-30h] BYREF

    bzero(szSQL, 0x1000u);
    snprintf((char *)szSQL, 0xFFFu, "SELECT * FROM `mem_char_wing` WHERE `cid`=%lld", nCid);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQL);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    if ( !Answer::MySqlQuery::eof(&result) )
    {
    this->m_Level = Answer::MySqlQuery::getIntValue(&result, "Level", 0);
    this->m_Luck = Answer::MySqlQuery::getIntValue(&result, "Luck", 0);
    this->m_HuanHua = Answer::MySqlQuery::getIntValue(&result, "HuanHua", 0);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    return 1;
}

void CharWingDBData::PackageData(Answer::NetPacket *packet)
{
    Answer::NetPacket::writeInt32(packet, this->m_Level);
    Answer::NetPacket::writeInt32(packet, this->m_Luck);
    Answer::NetPacket::writeInt32(packet, this->m_HuanHua);
}

void CharWingDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{
    this->m_Level = Answer::NetPacket::readInt32(inPacket);
    this->m_Luck = Answer::NetPacket::readInt32(inPacket);
    this->m_HuanHua = Answer::NetPacket::readInt32(inPacket);
}

（内容由AI生成，仅供参考）
