#include "Database/MailDBData.h"

MailDBData::MailDBData()
{
    /* base init: IDataStruct::IDataStruct(this); */
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D50F0; */
    this->m_MailInfo.map();
    /* base init: MailDBData::CleanUp(this); */
}

MailDBData::~MailDBData()
{
    /* vcall: this->_vptr_IDataStruct = (int (**)(...))&off_8D50F0; */
    std::map<int,MailInfo>::~map(&this->m_MailInfo);
}

void MailDBData::SaveToSqlString(SqlStringList *const sqls, char (*const szSQL)[4096], CharId_t nCid)
{
    std::_Rb_tree_iterator<std::pair<const int,MailInfo> > eiter; // [rsp+30h] [rbp-50h] BYREF
    std::_Rb_tree_iterator<std::pair<const int,MailInfo> > iter; // [rsp+40h] [rbp-40h] BYREF
    std::string __x; // [rsp+50h] [rbp-30h] BYREF

    int32_t CurTime; // [rsp+64h] [rbp-1Ch]
    MailInfo *Mail; // [rsp+68h] [rbp-18h]

    iter = this->m_MailInfo.begin();
    eiter = this->m_MailInfo.end();
    CurTime = Answer::DayTime::now();
    while ( std::_Rb_tree_iterator<std::pair<int const,MailInfo>>::operator!=(&iter, &eiter) )
    {
    Mail = &std::_Rb_tree_iterator<std::pair<int const,MailInfo>>::operator->(&iter)->second;
    bzero(szSQL, 0x1000u);
    if ( Mail->SendTime + 1209600 >= CurTime )
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "UPDATE `mem_mail` SET `HasRead`=%d,`Extract`=%d WHERE `MailId`=%d",
    Mail->HasRead,
    Mail->Extract,
    Mail->MailId);
    else
    snprintf(
    (char *)szSQL,
    0xFFFu,
    "UPDATE `mem_mail` SET `HasRead`=%d,`Extract`=%d, `DelFlag`=2,`DelTime`=%d WHERE `MailId`=%d",
    Mail->HasRead,
    Mail->Extract,
    CurTime,
    Mail->MailId);

    std::string::string(&__x, (char *)szSQL, &v8);
    std::list<std::string>::push_back(sqls, &__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(&v8);
    std::_Rb_tree_iterator<std::pair<int const,MailInfo>>::operator++(&iter);
    }
}

bool MailDBData::LoadFromDB(Answer::MySqlDBGuard *const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid)
{

    const char *StringValue; // rax
    const char *v8; // rax
    const char *v9; // rax
    const char *v10; // rax
    const char *v11; // rax
    const char *v12; // rax
    const char *v13; // rax
    const char *v14; // rax
    const char *v15; // rax
    const char *v16; // rax
    const char *v17; // rax


    CharId_t nCida; // [rsp+20h] [rbp-230h]
    int32_t nSida; // [rsp+2Ch] [rbp-224h]
    int64_t nUida; // [rsp+30h] [rbp-220h]
    char (*szSQLa)[4096]; // [rsp+38h] [rbp-218h]
    Answer::MySqlDBGuard *dba; // [rsp+40h] [rbp-210h]
    MailDBData *this; // [rsp+48h] [rbp-208h]
    MailInfo Mail; // [rsp+50h] [rbp-200h] BYREF
    std::_List_iterator<int> it; // [rsp+160h] [rbp-F0h] BYREF
    std::_List_iterator<int> iter; // [rsp+170h] [rbp-E0h] BYREF
    Answer::MySqlQuery result; // [rsp+180h] [rbp-D0h] BYREF
    Int32List NeedUpdateLoadFlag; // [rsp+1A0h] [rbp-B0h] BYREF
    Int32List autoDels; // [rsp+1B0h] [rbp-A0h] BYREF
    std::string p_ItemString; // [rsp+1C0h] [rbp-90h] BYREF


    std::_List_iterator<int> __x; // [rsp+220h] [rbp-30h] BYREF

    int32_t CurTime; // [rsp+238h] [rbp-18h]
    int32_t LoadFlg; // [rsp+23Ch] [rbp-14h]


    dba = db;
    szSQLa = szSQL;
    nUida = nUid;
    nSida = nSid;
    nCida = nCid;
    autoDels.list();
    NeedUpdateLoadFlag.list();
    bzero(szSQLa, 0x1000u);
    snprintf((char *)szSQLa, 0xFFFu, "SELECT * FROM `mem_mail` WHERE `ReceiveId`=%lld AND `DelFlag`=0", nCida);
    v6 = Answer::MySqlDBGuard::query(db, (const char *)szSQLa);
    Answer::MySqlQuery::MySqlQuery(&result, v6);
    CurTime = Answer::DayTime::now();
    while ( !Answer::MySqlQuery::eof(&result) )
    {
    memset(&Mail, 0, 268);
    std::string::string(&Mail.SenderName);
    std::string::string(&Mail.ReceiveName);
    std::string::string(&Mail.MailTitle);
    std::string::string(&Mail.MailContent);
    std::string::string(&Mail.Param);
    Mail.MailId = Answer::MySqlQuery::getIntValue(&result, "MailId", 0);
    Mail.SysMailId = Answer::MySqlQuery::getIntValue(&result, "SysMailId", 0);
    Mail.SenderId = Answer::MySqlQuery::getInt64Value(&result, "SenderId", 0);
    StringValue = Answer::MySqlQuery::getStringValue(&result, "SenderName", byte_8CFE00);
    std::string::operator=(&Mail.SenderName, StringValue);
    Mail.ReceiveId = Answer::MySqlQuery::getInt64Value(&result, "ReceiveId", 0);
    v8 = Answer::MySqlQuery::getStringValue(&result, "ReceiveName", byte_8CFE00);
    std::string::operator=(&Mail.ReceiveName, v8);
    Mail.SendTime = Answer::MySqlQuery::getIntValue(&result, "SendTime", 0);
    Mail.HasRead = Answer::MySqlQuery::getIntValue(&result, "HasRead", 0);
    Mail.Extract = Answer::MySqlQuery::getIntValue(&result, "Extract", 0);
    Mail.nReason = Answer::MySqlQuery::getIntValue(&result, "reason", 0);
    v9 = Answer::MySqlQuery::getStringValue(&result, "Param", byte_8CFE00);
    std::string::operator=(&Mail.Param, v9);
    v10 = Answer::MySqlQuery::getStringValue(&result, "MailTitle", byte_8CFE00);
    std::string::operator=(&Mail.MailTitle, v10);
    v11 = Answer::MySqlQuery::getStringValue(&result, "MailContent", byte_8CFE00);
    std::string::operator=(&Mail.MailContent, v11);
    LoadFlg = Answer::MySqlQuery::getIntValue(&result, "LoadFlag", 0);

    v12 = Answer::MySqlQuery::getStringValue(&result, "Item1", byte_8CFE00);
    std::string::string(&p_ItemString, v12, &v36);
    MailDBData::ParesChrBagString(&v22, this, &p_ItemString);
    Mail.Item[0] = v22;
    std::string::~string(&p_ItemString);
    std::allocator<char>::~allocator(&v36);

    v13 = Answer::MySqlQuery::getStringValue(&result, "Item2", byte_8CFE00);
    std::string::string(&v37, v13, &v38);
    MailDBData::ParesChrBagString(&v22, this, &v37);
    Mail.Item[1] = v22;
    std::string::~string(&v37);
    std::allocator<char>::~allocator(&v38);

    v14 = Answer::MySqlQuery::getStringValue(&result, "Item3", byte_8CFE00);
    std::string::string(&v39, v14, &v40);
    MailDBData::ParesChrBagString(&v22, this, &v39);
    Mail.Item[2] = v22;
    std::string::~string(&v39);
    std::allocator<char>::~allocator(&v40);

    v15 = Answer::MySqlQuery::getStringValue(&result, "Item4", byte_8CFE00);
    std::string::string(&v41, v15, &v42);
    MailDBData::ParesChrBagString(&v22, this, &v41);
    Mail.Item[3] = v22;
    std::string::~string(&v41);
    std::allocator<char>::~allocator(&v42);

    v16 = Answer::MySqlQuery::getStringValue(&result, "Item5", byte_8CFE00);
    std::string::string(&v43, v16, &v44);
    MailDBData::ParesChrBagString(&v22, this, &v43);
    Mail.Item[4] = v22;
    std::string::~string(&v43);
    std::allocator<char>::~allocator(&v44);

    v17 = Answer::MySqlQuery::getStringValue(&result, "Item6", byte_8CFE00);
    std::string::string(&v45, v17, &v46);
    MailDBData::ParesChrBagString(&v22, this, &v45);
    Mail.Item[5] = v22;
    std::string::~string(&v45);
    std::allocator<char>::~allocator(&v46);
    if ( Mail.SendTime + 1209600 >= CurTime )
    {
    v18 = std::map<int,MailInfo>::operator[](&this->m_MailInfo, &Mail.MailId);
    MailInfo::operator=(v18, &Mail);
    }
    else
    {
    autoDels.push_back(&Mail.MailId);
    }
    if ( LoadFlg <= 0 )
    NeedUpdateLoadFlag.push_back(&Mail.MailId);
    Answer::MySqlQuery::nextRow(&result);
    MailInfo::~MailInfo(&Mail);
    }
    if ( !autoDels.empty() )
    {
    for ( iter = autoDels.begin(); ; std::_List_iterator<int>::operator++(&iter) )
    {
    __x = autoDels.end();
    if ( !std::_List_iterator<int>::operator!=(&iter, &__x) )
    break;
    bzero(szSQLa, 0x1000u);
    v19 = std::_List_iterator<int>::operator*(&iter);
    snprintf(
    (char *)szSQLa,
    0xFFFu,
    "UPDATE `mem_mail` set `DelFlag`=2,`DelTime`=%d WHERE `MailId`=%d",
    CurTime,
    *v19);
    Answer::MySqlDBGuard::excute(dba, (const char *)szSQLa);
    }
    }
    if ( !NeedUpdateLoadFlag.empty() )
    {
    for ( it = NeedUpdateLoadFlag.begin(); ; std::_List_iterator<int>::operator++(&it) )
    {
    v48 = NeedUpdateLoadFlag.end();
    if ( !std::_List_iterator<int>::operator!=(&it, &v48) )
    break;
    bzero(szSQLa, 0x1000u);
    v20 = std::_List_iterator<int>::operator*(&it);
    snprintf((char *)szSQLa, 0xFFFu, "UPDATE `mem_mail` SET `LoadFlag`=1 WHERE `MailId`=%d", *v20);
    Answer::MySqlDBGuard::excute(dba, (const char *)szSQLa);
    }
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    std::list<int>::~list(&NeedUpdateLoadFlag);
    std::list<int>::~list(&autoDels);
    return 1;
}

void MailDBData::PackageData(Answer::NetPacket *packet)
{
    std::pair<const int,MailInfo> *v2; // rax
    char *Buffer; // rax
    MailInfo Mail; // [rsp+10h] [rbp-150h] BYREF
    std::_Rb_tree_iterator<std::pair<const int,MailInfo> > it; // [rsp+120h] [rbp-40h] BYREF
    std::_Rb_tree_iterator<std::pair<const int,MailInfo> > __x; // [rsp+130h] [rbp-30h] BYREF
    int32_t nSize; // [rsp+140h] [rbp-20h]
    int8_t Count; // [rsp+147h] [rbp-19h]
    int32_t Offset; // [rsp+148h] [rbp-18h]
    int8_t i; // [rsp+14Fh] [rbp-11h]

    if ( packet )
    {
    nSize = this->m_MailInfo.size();
    Answer::NetPacket::writeInt32(packet, nSize);
    for ( it = this->m_MailInfo.begin();
    ;
    std::_Rb_tree_iterator<std::pair<int const,MailInfo>>::operator++(&it) )
    {
    __x = this->m_MailInfo.end();
    if ( !std::_Rb_tree_iterator<std::pair<int const,MailInfo>>::operator!=(&it, &__x) )
    break;
    v2 = std::_Rb_tree_iterator<std::pair<int const,MailInfo>>::operator->(&it);
    MailInfo::MailInfo(&Mail, &v2->second);
    Answer::NetPacket::writeInt32(packet, Mail.MailId);
    Answer::NetPacket::writeInt32(packet, Mail.SysMailId);
    Answer::NetPacket::writeInt64(packet, Mail.SenderId);
    Answer::NetPacket::writeUTF8(packet, &Mail.SenderName);
    Answer::NetPacket::writeInt64(packet, Mail.ReceiveId);
    Answer::NetPacket::writeUTF8(packet, &Mail.ReceiveName);
    Answer::NetPacket::writeInt32(packet, Mail.SendTime);
    Answer::NetPacket::writeInt8(packet, Mail.HasRead);
    Answer::NetPacket::writeInt8(packet, Mail.Extract);
    Answer::NetPacket::writeUTF8(packet, &Mail.MailTitle);
    Answer::NetPacket::writeUTF8(packet, &Mail.MailContent);
    Answer::NetPacket::writeInt32(packet, Mail.nReason);
    Answer::NetPacket::writeUTF8(packet, &Mail.Param);
    if ( Mail.Extract == 1 || Mail.Extract == 2 )
    {
    Count = 0;
    Offset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::writeInt8(packet, Count);
    for ( i = 0; i <= 5; ++i )
    {
    if ( Mail.Item[i].itemId > 0 && Mail.Item[i].itemCount > 0 )
    {
    Answer::NetPacket::writeInt8(packet, i);
    Answer::NetPacket::writeInt32(packet, Mail.Item[i].itemId);
    Answer::NetPacket::writeInt8(packet, Mail.Item[i].itemClass);
    Answer::NetPacket::writeInt32(packet, Mail.Item[i].itemCount);
    Answer::NetPacket::writeInt8(packet, Mail.Item[i].bind);
    Answer::NetPacket::writeInt32(packet, Mail.Item[i].endTime);
    Answer::NetPacket::writeInt64(packet, Mail.Item[i].srcId);
    ++Count;
    }
    }
    Buffer = Answer::NetPacket::getBuffer(packet);
    Buffer[Offset] = Count;
    }
    MailInfo::~MailInfo(&Mail);
    }
    }
}

void MailDBData::UnPackageData(Answer::NetPacket *inPacket, CharId_t nCid)
{


    MailInfo Mail; // [rsp+20h] [rbp-180h] BYREF

    _BYTE v13[20]; // [rsp+170h] [rbp-30h] BYREF
    int32_t nSize; // [rsp+184h] [rbp-1Ch]
    int32_t i; // [rsp+188h] [rbp-18h]
    int8_t Count; // [rsp+18Dh] [rbp-13h]
    int8_t i_0; // [rsp+18Eh] [rbp-12h]
    int8_t Index; // [rsp+18Fh] [rbp-11h]

    if ( inPacket )
    {
    nSize = Answer::NetPacket::readInt32(inPacket);
    for ( i = 0; ; ++i )
    {
    if ( i >= nSize )
    return;
    memset(&Mail, 0, 268);
    std::string::string(&Mail.SenderName);
    std::string::string(&Mail.ReceiveName);
    std::string::string(&Mail.MailTitle);
    std::string::string(&Mail.MailContent);
    std::string::string(&Mail.Param);
    Mail.MailId = Answer::NetPacket::readInt32(inPacket);
    Mail.SysMailId = Answer::NetPacket::readInt32(inPacket);
    Mail.SenderId = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8(&v12, (bool)inPacket);
    std::string::operator=(&Mail.SenderName, &v12);
    std::string::~string(&v12);
    Mail.ReceiveId = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&v12.m_wOffset, (bool)inPacket);
    std::string::operator=(&Mail.ReceiveName, &v12.m_wOffset);
    std::string::~string(&v12.m_wOffset);
    Mail.SendTime = Answer::NetPacket::readInt32(inPacket);
    Mail.HasRead = Answer::NetPacket::readInt8(inPacket);
    Mail.Extract = Answer::NetPacket::readInt8(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&v12.m_size, (bool)inPacket);
    std::string::operator=(&Mail.MailTitle, &v12.m_size);
    std::string::~string(&v12.m_size);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&v12.m_pool, (bool)inPacket);
    std::string::operator=(&Mail.MailContent, &v12.m_pool);
    std::string::~string(&v12.m_pool);
    Mail.nReason = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)v13, (bool)inPacket);
    std::string::operator=(&Mail.Param, v13);
    std::string::~string(v13);
    if ( Mail.Extract == 1 || Mail.Extract == 2 )
    {
    Count = Answer::NetPacket::readInt8(inPacket);
    if ( Count > 6 )
    {
    v3 = 0;
    goto LABEL_11;
    }
    for ( i_0 = 0; i_0 < Count; ++i_0 )
    {
    Index = Answer::NetPacket::readInt8(inPacket);
    v4 = Index;
    Mail.Item[v4].itemId = Answer::NetPacket::readInt32(inPacket);
    v5 = Index;
    Mail.Item[v5].itemClass = Answer::NetPacket::readInt8(inPacket);
    v6 = Index;
    Mail.Item[v6].itemCount = Answer::NetPacket::readInt32(inPacket);
    v7 = Index;
    Mail.Item[v7].bind = Answer::NetPacket::readInt8(inPacket);
    v8 = Index;
    Mail.Item[v8].endTime = Answer::NetPacket::readInt32(inPacket);
    v9 = Index;
    Mail.Item[v9].srcId = Answer::NetPacket::readInt64(inPacket);
    }
    }
    v10 = std::map<int,MailInfo>::operator[](&this->m_MailInfo, &Mail.MailId);
    MailInfo::operator=(v10, &Mail);
    v3 = 1;
    LABEL_11:
    MailInfo::~MailInfo(&Mail);
    if ( !v3 )
    return;
    }
    }
}

（内容由AI生成，仅供参考）
