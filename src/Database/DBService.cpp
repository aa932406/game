#include "Database/DBService.h"

DBService::DBService()
{
    /* vcall: this->_vptr_DBService = (int (**)(...))off_8D4950; */
    std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::map(&this->m_mConn);
}

DBService::~DBService()
{
    /* vcall: this->_vptr_DBService = (int (**)(...))off_8D4950; */
    std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::~map(&this->m_mConn);
}

void DBService::Init(int32_t line)
{
    if ( line == 9 )
    /* base init: DBService::onLoadGMBackstage(this); */
}

bool DBService::Connect(int8_t id, std::string *p_host, int32_t port)
{
    std::map<signed char,DBService::ConnType*>::ConnType *v4; // rbx

    std::map<signed char,DBService::ConnType*>::ConnType **v7; // rax
    uint32_t WOffset; // eax

    int8_t ida[4]; // [rsp+34h] [rbp-6Ch] BYREF
    DBService *this; // [rsp+38h] [rbp-68h]
    std::string name; // [rsp+40h] [rbp-60h] BYREF

    Answer::InetAddress serverAddr; // [rsp+50h] [rbp-50h] BYREF
    std::string val; // [rsp+60h] [rbp-40h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+70h] [rbp-30h]
    Answer::NetPacket *packet; // [rsp+78h] [rbp-28h]


    porta = port;
    ida[0] = id;

    std::string::string(&name, "DBService", &v14);
    v4 = (std::map<signed char,DBService::ConnType*>::ConnType *)operator new(0x118u);
    MutiConn<DBService>::MutiConn(v4, 0x2000, 0x2000, 0x3FF9u, 0, 0x7D0u, (std::string)&name, id, this);
    pConn = v4;
    std::string::~string(&name);
    std::allocator<char>::~allocator(&v14);
    if ( !pConn )
    return 0;
    Answer::InetAddress::InetAddress(&serverAddr, p_host, porta);
    v6 = !Answer::TcpClient::connect(pConn, &serverAddr);
    Answer::InetAddress::~InetAddress(&serverAddr);
    if ( v6 )
    return 0;
    Answer::Thread::start(pConn);
    v7 = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::operator[](
    &this->m_mConn,
    ida);
    *v7 = pConn;
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( !packet )
    return 0;
    Answer::TcpClient::getName((Answer::TcpClient *const)&val);
    Answer::NetPacket::writeUTF8(packet, &val);
    std::string::~string(&val);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EC6u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    return 1;
}

void DBService::onNetPacket(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    if ( pConn && inPacket )
    {
    switch ( Answer::NetPacket::getProc(inPacket) )
    {
    case 0x4EC9u:
    this->onDBPlayerLoaded(pConn, inPacket);
    break;
    case 0x4ECCu:
    this->onDBPlayerSaved(pConn, inPacket);
    break;
    case 0x4ED0u:
    this->onDBEquipUpdated(pConn, inPacket);
    break;
    case 0x4ED1u:
    this->onUpdateMYSJReward(pConn, inPacket);
    break;
    case 0x4ED3u:
    this->onCityWarInfoLoaded(pConn, inPacket);
    break;
    case 0x4ED7u:
    this->onPeerlessWarInfoLoaded(pConn, inPacket);
    break;
    case 0x4ED8u:
    this->onCrossTowerInfoLoaded(pConn, inPacket);
    break;
    case 0x4ED9u:
    this->onServerBattleLoaded(pConn, inPacket);
    break;
    case 0x4EDEu:
    this->onDBUserPayed(pConn, inPacket);
    break;
    case 0x4EEEu:
    this->OnUpdateMailInfo(pConn, inPacket);
    break;
    case 0x4EF3u:
    this->onUpdateCityWarResult(pConn, inPacket);
    break;
    case 0x4EF4u:
    this->onUpdateKaiFuTime(pConn, inPacket);
    break;
    case 0x4EF7u:
    this->onUpdateGMBroadcast(pConn, inPacket);
    break;
    case 0x4EF8u:
    this->onUpdateGMBanChat(pConn, inPacket);
    break;
    case 0x4EF9u:
    this->onUpdateGMSeal(pConn, inPacket);
    break;
    case 0x4EFAu:
    this->onBackStageKick(pConn, inPacket);
    break;
    case 0x4EFCu:
    this->onUpdateCrossTowerResutl(pConn, inPacket);
    break;
    case 0x4EFEu:
    this->onUpdatePeerlessWarResult(pConn, inPacket);
    break;
    case 0x4EFFu:
    this->onUpdateBossFirstKilled(pConn, inPacket);
    break;
    case 0x4F08u:
    this->onUpdateWorldBossInfo(pConn, inPacket);
    break;
    case 0x4F09u:
    this->onUpdateEquipBackRecord(pConn, inPacket);
    break;
    case 0x4F0Au:
    this->onUpdateEquipBackInfo(pConn, inPacket);
    break;
    case 0x4F0Du:
    this->onUpdateDoubleElevenRank(pConn, inPacket);
    break;
    case 0x4F10u:
    this->onUpdateZHYYHDRank(pConn, inPacket);
    break;
    case 0x4F13u:
    this->onUpdateDropTimes(pConn, inPacket);
    break;
    case 0x4F1Fu:
    this->onUpdateTencentInfo(pConn, inPacket);
    break;
    default:
    this->onPlayerNetPacket(pConn, inPacket);
    break;
    }
    Answer::NetPacket::destroy(inPacket);
    }
}

void DBService::savePlayer(int8_t connid, int32_t reason, int32_t param, PlayerDBData *const dbData)
{

    int32_t Id; // edx
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, reason);
    Answer::NetPacket::writeInt32(packet, param);
    v5 = Answer::Singleton<GameService>::instance();
    Id = GameService::getId(v5);
    Answer::NetPacket::writeInt32(packet, Id);
    /* vcall: (*((void (__fastcall **)(PlayerDBData *const, Answer::NetPacket *))dbData->_vptr_IDataStruct + 2))(dbData, packet); */
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ECAu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::loadPlayer(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int32_t reason)
{

    int32_t Id; // edx
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt8(packet, connid);
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt32(packet, reason);
    v6 = Answer::Singleton<GameService>::instance();
    Id = GameService::getId(v6);
    Answer::NetPacket::writeInt32(packet, Id);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EC8u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::fengHao(int8_t connid, int8_t Type, int64_t Cid, std::string *p_name)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt8(packet, Type);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EC7u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::UpdateLoginTime(int8_t connid, CharId_t cid, int32_t login_time)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, login_time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EF5u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogPlayerStay(int8_t connid, int32_t nTime, int32_t nDay)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt32(packet, nDay);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EF6u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::insertMemEquip(int8_t connid, const MemEquip *const equip, int32_t nReason)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nReason);
    MemEquip::PackageData(equip, packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ECDu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::updateMemEquip(int8_t connid, const MemEquip *const equip, int32_t nReason)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nReason);
    MemEquip::PackageData(equip, packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ECEu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::deleteMemEquip(int8_t connid, const MemEquip *const equip, int32_t nReason)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nReason);
    MemEquip::PackageData(equip, packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ECFu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::insertTaskInfo(int8_t connid, const LogTask *const task)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, task->cid);
    Answer::NetPacket::writeInt32(packet, task->tid);
    Answer::NetPacket::writeInt32(packet, task->type);
    Answer::NetPacket::writeInt32(packet, task->time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EDFu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::updateTaskInfo(int8_t connid, const LogTask *const task)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, task->cid);
    Answer::NetPacket::writeInt32(packet, task->tid);
    Answer::NetPacket::writeInt32(packet, task->type);
    Answer::NetPacket::writeInt32(packet, task->time);
    Answer::NetPacket::writeInt32(packet, task->state);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE0u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::insertDungeon(int8_t connid, const LogDungeon *const dungeon)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, dungeon->cid);
    Answer::NetPacket::writeInt32(packet, dungeon->did);
    Answer::NetPacket::writeInt32(packet, dungeon->type);
    Answer::NetPacket::writeInt32(packet, dungeon->level);
    Answer::NetPacket::writeInt32(packet, dungeon->start_time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE1u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::updateDungeon(int8_t connid, const LogDungeon *const dungeon)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, dungeon->cid);
    Answer::NetPacket::writeInt32(packet, dungeon->did);
    Answer::NetPacket::writeInt32(packet, dungeon->state);
    Answer::NetPacket::writeInt32(packet, dungeon->start_time);
    Answer::NetPacket::writeInt32(packet, dungeon->finish_time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE2u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveWorldBossInfo(const WorldBossInfo *const info)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    WorldBossInfo::PackageBossInfo(info, packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F00u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdateWorldBossInfo(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    WorldBossInfo info; // [rsp+20h] [rbp-B0h] BYREF

    if ( pConn )
    {
    if ( inPacket )
    {
    WorldBossInfo::WorldBossInfo(&info);
    WorldBossInfo::UnPackageBossInfo(&info, inPacket);
    v3 = Answer::Singleton<CWorldBoss>::instance();
    CWorldBoss::UpdateWorldBossInfo(v3, &info);
    WorldBossInfo::~WorldBossInfo(&info);
    }
    }
}

void DBService::onUpdateEquipBackRecord(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    std::string sName; // [rsp+20h] [rbp-40h] BYREF
    std::string p_Name; // [rsp+30h] [rbp-30h] BYREF
    int32_t nId; // [rsp+44h] [rbp-1Ch]
    int8_t nType; // [rsp+4Bh] [rbp-15h]
    int32_t nTime; // [rsp+4Ch] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nId = Answer::NetPacket::readInt32(inPacket);
    nType = Answer::NetPacket::readInt8(inPacket);
    nTime = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&sName, (bool)inPacket);
    std::string::string(&p_Name, &sName);
    v3 = nType;
    v4 = Answer::Singleton<CEquipBack>::instance();
    CEquipBack::UpdateEquipRecord(v4, nId, v3, nTime, &p_Name);
    std::string::~string(&p_Name);
    std::string::~string(&sName);
    }
    }
}

void DBService::onUpdateEquipBackInfo(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    std::string sName; // [rsp+20h] [rbp-40h] BYREF
    std::string p_Name; // [rsp+30h] [rbp-30h] BYREF
    int32_t nId; // [rsp+44h] [rbp-1Ch]
    int8_t nType; // [rsp+4Bh] [rbp-15h]
    int32_t nCount; // [rsp+4Ch] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nId = Answer::NetPacket::readInt32(inPacket);
    nType = Answer::NetPacket::readInt8(inPacket);
    nCount = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&sName, (bool)inPacket);
    std::string::string(&p_Name, &sName);
    v3 = nType;
    v4 = Answer::Singleton<CEquipBack>::instance();
    CEquipBack::UpdateEquipBackInfo(v4, nId, v3, nCount, &p_Name);
    std::string::~string(&p_Name);
    std::string::~string(&sName);
    }
    }
}

void DBService::LoadDoubleElevenRank(int8_t nType, int8_t nDay)
{
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( !packet )
    return;
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt8(packet, nDay);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F0Bu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveDoubleElevenRank(int8_t connid, int8_t nType, int16_t nDay, int16_t nIndex, const FestivalRank *const info)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt16(packet, nDay);
    Answer::NetPacket::writeInt16(packet, nIndex);
    Answer::NetPacket::writeInt64(packet, info->nCharId);
    Answer::NetPacket::writeUTF8(packet, &info->strName);
    Answer::NetPacket::writeInt32(packet, info->nScore);
    Answer::NetPacket::writeInt32(packet, info->nTime);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F0Cu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdateDoubleElevenRank(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    int16_t type; // r13
    int8_t m_policy; // r12
    int8_t Id; // bl

    FestivalRank info; // [rsp+20h] [rbp-70h] BYREF


    if ( pConn && inPacket )
    {
    FestivalRank::FestivalRank(&info);
    v10.m_policy = Answer::NetPacket::readInt8(inPacket);
    LOWORD(v10.type) = Answer::NetPacket::readInt16(inPacket);
    HIWORD(v10.type) = Answer::NetPacket::readInt16(inPacket);
    for ( v10.m_proc = 0; (__int16)v10.m_proc < SHIWORD(v10.type); ++v10.m_proc )
    {
    *(&v10.m_proc + 1) = Answer::NetPacket::readInt16(inPacket);
    info.nCharId = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8(&v10, (bool)inPacket);
    std::string::operator=(&info.strName, &v10);
    std::string::~string(&v10);
    info.nScore = Answer::NetPacket::readInt32(inPacket);
    info.nTime = Answer::NetPacket::readInt32(inPacket);
    v3 = *(&v10.m_proc + 1);
    type = v10.type;
    m_policy = v10.m_policy;
    Id = MutiConn<DBService>::GetId(pConn);
    v7 = Answer::Singleton<CFestivalDoubleEleven>::instance();
    CFestivalDoubleEleven::UpdateRank(v7, Id, m_policy, type, v3, &info);
    }
    FestivalRank::~FestivalRank(&info);
    }
}

void DBService::LoadZHYYHDRank(int8_t nType, int8_t nDay)
{
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( !packet )
    return;
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt8(packet, nDay);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F0Eu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveZHYYHDRank(int8_t connid, int8_t nType, int16_t nDay, const FestivalRankVector *const vRank)
{

    const FestivalRank *v6; // rax
    const FestivalRank *v7; // rax
    const FestivalRank *v8; // rax
    const FestivalRank *v9; // rax
    const FestivalRank *v10; // rax
    uint32_t WOffset; // eax
    __gnu_cxx::__normal_iterator<const FestivalRank*,std::vector<FestivalRank> > iter; // [rsp+20h] [rbp-40h] BYREF
    __gnu_cxx::__normal_iterator<const FestivalRank*,std::vector<FestivalRank> > __rhs; // [rsp+30h] [rbp-30h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+40h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+48h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt16(packet, nDay);
    v5 = std::vector<FestivalRank>::size(vRank);
    Answer::NetPacket::writeInt16(packet, v5);
    for ( iter = std::vector<FestivalRank>::begin(vRank);
    ;
    __gnu_cxx::__normal_iterator<FestivalRank const*,std::vector<FestivalRank>>::operator++(&iter) )
    {
    __rhs = std::vector<FestivalRank>::end(vRank);
    if ( !__gnu_cxx::operator!=<FestivalRank const*,std::vector<FestivalRank>>(&iter, &__rhs) )
    break;
    v6 = __gnu_cxx::__normal_iterator<FestivalRank const*,std::vector<FestivalRank>>::operator->(&iter);
    Answer::NetPacket::writeInt16(packet, v6->nIndex);
    v7 = __gnu_cxx::__normal_iterator<FestivalRank const*,std::vector<FestivalRank>>::operator->(&iter);
    Answer::NetPacket::writeInt64(packet, v7->nCharId);
    v8 = __gnu_cxx::__normal_iterator<FestivalRank const*,std::vector<FestivalRank>>::operator->(&iter);
    Answer::NetPacket::writeUTF8(packet, &v8->strName);
    v9 = __gnu_cxx::__normal_iterator<FestivalRank const*,std::vector<FestivalRank>>::operator->(&iter);
    Answer::NetPacket::writeInt32(packet, v9->nScore);
    v10 = __gnu_cxx::__normal_iterator<FestivalRank const*,std::vector<FestivalRank>>::operator->(&iter);
    Answer::NetPacket::writeInt32(packet, v10->nTime);
    }
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F0Fu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdateZHYYHDRank(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    int8_t Id; // bl

    FestivalRankVector vRank; // [rsp+20h] [rbp-80h] BYREF
    FestivalRank p___x; // [rsp+40h] [rbp-60h] BYREF
    _BYTE v15[9]; // [rsp+70h] [rbp-30h] BYREF
    int8_t nType; // [rsp+79h] [rbp-27h]
    int16_t nDay; // [rsp+7Ah] [rbp-26h]
    int16_t nSize; // [rsp+7Ch] [rbp-24h]
    int16_t i; // [rsp+7Eh] [rbp-22h]

    if ( pConn && inPacket )
    {
    vRank.vector();
    nType = Answer::NetPacket::readInt8(inPacket);
    nDay = Answer::NetPacket::readInt16(inPacket);
    nSize = Answer::NetPacket::readInt16(inPacket);
    FestivalRank::FestivalRank(&p___x);
    vRank.resize(nSize, &p___x);
    FestivalRank::~FestivalRank(&p___x);
    for ( i = 0; i < nSize; ++i )
    {
    v3 = std::vector<FestivalRank>::operator[](&vRank, i);
    v3->nIndex = Answer::NetPacket::readInt16(inPacket);
    v4 = std::vector<FestivalRank>::operator[](&vRank, i);
    v4->nCharId = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)v15, (bool)inPacket);
    v5 = std::vector<FestivalRank>::operator[](&vRank, i);
    std::string::operator=(&v5->strName, v15);
    std::string::~string(v15);
    v6 = std::vector<FestivalRank>::operator[](&vRank, i);
    v6->nScore = Answer::NetPacket::readInt32(inPacket);
    v7 = std::vector<FestivalRank>::operator[](&vRank, i);
    v7->nTime = Answer::NetPacket::readInt32(inPacket);
    }
    v8 = nDay;
    v9 = nType;
    Id = MutiConn<DBService>::GetId(pConn);
    v11 = Answer::Singleton<CZongHeYunYingHD>::instance();
    CZongHeYunYingHD::UpdateRankInfo(v11, Id, v9, v8, &vRank);
    std::vector<FestivalRank>::~vector(&vRank);
    }
}

void DBService::SaveDropTimes(int32_t nGroupId, int32_t nTimes)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nGroupId);
    Answer::NetPacket::writeInt32(packet, nTimes);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F11u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::ClearDropTimes()
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F12u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdateDropTimes(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    int32_t nGroupId; // [rsp+28h] [rbp-8h]
    int32_t nTimes; // [rsp+2Ch] [rbp-4h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nGroupId = Answer::NetPacket::readInt32(inPacket);
    nTimes = Answer::NetPacket::readInt32(inPacket);
    v3 = Answer::Singleton<GameService>::instance();
    GameService::UpdateDropTimes(v3, nGroupId, nTimes);
    }
    }
}

void DBService::onUpdateMYSJReward(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    const char *v3; // rax

    LogMoYuShiJieReward logInfo; // [rsp+20h] [rbp-60h] BYREF


    if ( pConn )
    {
    if ( inPacket )
    {
    memset(&logInfo, 0, sizeof(logInfo));
    logInfo.cid = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&v7, (bool)inPacket);
    v3 = (const char *)std::string::c_str(&v7);
    snprintf(logInfo.name, 0x1Eu, v3);
    std::string::~string(&v7);
    logInfo.dungeonid = Answer::NetPacket::readInt32(inPacket);
    logInfo.itemid = Answer::NetPacket::readInt32(inPacket);
    logInfo.itemclass = Answer::NetPacket::readInt8(inPacket);
    logInfo.itemcount = Answer::NetPacket::readInt32(inPacket);
    logInfo.time = Answer::NetPacket::readInt32(inPacket);
    logInfo.special = Answer::NetPacket::readInt8(inPacket);
    v4 = Answer::Singleton<GameService>::instance();
    GameService::AddMoYuShiJieRecord(v4, &logInfo);
    }
    }
}

void DBService::SaveDropRecord(const DropRecord *const record)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeUTF8(packet, &record->strName);
    Answer::NetPacket::writeInt64(packet, record->nCharId);
    Answer::NetPacket::writeInt32(packet, record->nMapId);
    Answer::NetPacket::writeInt32(packet, record->nMid);
    Answer::NetPacket::writeInt32(packet, record->nRecord);
    Answer::NetPacket::writeInt32(packet, record->nTime);
    Answer::NetPacket::writeInt8(packet, record->nSpecial);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F14u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveBossFirstKilled(int32_t BossId, CharId_t Cid, std::string *p_Name, int32_t time)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeUTF8(packet, p_Name);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeInt32(packet, BossId);
    Answer::NetPacket::writeInt32(packet, time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F15u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::InsertBossLog(int8_t connid, const LogBoss *const boss)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, boss->mid);
    Answer::NetPacket::writeInt32(packet, boss->type);
    Answer::NetPacket::writeInt64(packet, boss->killer);
    Answer::NetPacket::writeInt32(packet, boss->time);
    Answer::NetPacket::writeInt32(packet, boss->mapId);
    Answer::NetPacket::writeInt32(packet, boss->MapMonsterId);
    Answer::NetPacket::writeInt64(packet, boss->EntityId);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE7u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::InsertActivityLog(int8_t connid, const LogActivity *const activity)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, activity->cid);
    Answer::NetPacket::writeInt32(packet, activity->actid);
    Answer::NetPacket::writeInt32(packet, activity->acttype);
    Answer::NetPacket::writeInt32(packet, activity->time);
    Answer::NetPacket::writeInt64(packet, activity->param);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE4u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::InsertDailyGiftLog(int8_t connid, const LogDailyGift *const gift)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, gift->cid);
    Answer::NetPacket::writeInt32(packet, gift->type);
    Answer::NetPacket::writeInt32(packet, gift->giftid);
    Answer::NetPacket::writeInt32(packet, gift->time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE8u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::insertItemChange(int8_t connid, const LogItemChange *const item)
{
    uint32_t WOffset; // eax
    std::string val; // [rsp+20h] [rbp-30h] BYREF

    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, item->cid);

    std::string::string(&val, item->name, &v6);
    Answer::NetPacket::writeUTF8(packet, &val);
    std::string::~string(&val);
    std::allocator<char>::~allocator(&v6);
    Answer::NetPacket::writeInt8(packet, item->flag);
    Answer::NetPacket::writeInt32(packet, item->reason);
    Answer::NetPacket::writeInt32(packet, item->id);
    Answer::NetPacket::writeInt8(packet, item->itemClass);
    Answer::NetPacket::writeInt32(packet, item->count);
    Answer::NetPacket::writeInt32(packet, item->time);
    Answer::NetPacket::writeInt64(packet, item->srcId);
    Answer::NetPacket::writeInt32(packet, item->allCount);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE5u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::insertMingGeItem(int8_t connid, const MGLog *const logStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    MGLog::PackageData(logStu, packet);
    Answer::NetPacket::setProc(packet, 0x4EE6u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::logLevelUp(int8_t connid, const LogPlayerLevel *const logLevel)
{
    uint32_t WOffset; // eax
    std::string val; // [rsp+20h] [rbp-30h] BYREF

    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, logLevel->cid);

    std::string::string(&val, logLevel->name, &v6);
    Answer::NetPacket::writeUTF8(packet, &val);
    std::string::~string(&val);
    std::allocator<char>::~allocator(&v6);
    Answer::NetPacket::writeInt32(packet, logLevel->level);
    Answer::NetPacket::writeInt32(packet, logLevel->time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EDDu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveOnlineTime(int8_t connid, CharId_t nCharId, int32_t nDayTime, int32_t nOnlineTime)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, nCharId);
    Answer::NetPacket::writeInt32(packet, nDayTime);
    Answer::NetPacket::writeInt32(packet, nOnlineTime);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F1Du);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::log360(int8_t connid, Log360 *const LogStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeUTF8(packet, &LogStu->Interface);
    Answer::NetPacket::writeInt32(packet, LogStu->UseSid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->passport);
    Answer::NetPacket::writeUTF8(packet, &LogStu->LogString);
    Answer::NetPacket::writeInt64(packet, LogStu->Cid);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F16u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogProc(int8_t connid, PacketProcLog *const stu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, stu->nCid);
    Answer::NetPacket::writeUTF8(packet, &stu->nName);
    Answer::NetPacket::writeInt32(packet, stu->nProcId);
    Answer::NetPacket::writeInt32(packet, stu->nTime);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F17u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::logGeneral(int8_t connid, Generallog *const LogStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, LogStu->Time);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sString);
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F18u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogUpEquipStar(int8_t connid, UpStartLog *const LogStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->nName);
    Answer::NetPacket::writeInt32(packet, LogStu->nNewStar);
    Answer::NetPacket::writeInt32(packet, LogStu->nEquipId);
    Answer::NetPacket::writeInt64(packet, LogStu->nSrcId);
    Answer::NetPacket::writeInt32(packet, LogStu->nOpway);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F1Au);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogEquipDrop(int8_t connid, DropLog *const LogStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sName);
    Answer::NetPacket::writeInt32(packet, LogStu->nEquipId);
    Answer::NetPacket::writeInt64(packet, LogStu->nSrcId);
    Answer::NetPacket::writeInt32(packet, LogStu->nMapId);
    Answer::NetPacket::writeInt64(packet, LogStu->nKiller);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sKillerName);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F1Bu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogChangeMap(int8_t connid, ChangeMapLog *const LogStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sName);
    Answer::NetPacket::writeInt32(packet, LogStu->nCurMapId);
    Answer::NetPacket::writeInt32(packet, LogStu->nCurX);
    Answer::NetPacket::writeInt32(packet, LogStu->nCurY);
    Answer::NetPacket::writeInt32(packet, LogStu->nTarMapId);
    Answer::NetPacket::writeInt32(packet, LogStu->nTarX);
    Answer::NetPacket::writeInt32(packet, LogStu->nTarY);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F1Cu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogNpcFunc(int8_t connid, NpcFuncLog *p_LogStu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, p_LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &p_LogStu->sName);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nFuncId);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nMapId);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nX);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nY);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nTime);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F19u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::logMoYuShiJieReward(int8_t connid, const LogMoYuShiJieReward *const logInfo)
{
    uint32_t WOffset; // eax
    std::string val; // [rsp+20h] [rbp-30h] BYREF

    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, logInfo->cid);

    std::string::string(&val, logInfo->name, &v6);
    Answer::NetPacket::writeUTF8(packet, &val);
    std::string::~string(&val);
    std::allocator<char>::~allocator(&v6);
    Answer::NetPacket::writeInt32(packet, logInfo->dungeonid);
    Answer::NetPacket::writeInt32(packet, logInfo->itemid);
    Answer::NetPacket::writeInt8(packet, logInfo->itemclass);
    Answer::NetPacket::writeInt32(packet, logInfo->itemcount);
    Answer::NetPacket::writeInt32(packet, logInfo->time);
    Answer::NetPacket::writeInt8(packet, logInfo->special);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EDAu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::logPlayerLogin(int8_t connid, CharId_t cid, int32_t action, int32_t time, int32_t MapId)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, action);
    Answer::NetPacket::writeInt32(packet, time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EDBu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::logCurrency(int8_t connid, CharId_t cid, const std::string *const name, int32_t nType, int32_t opWay, int32_t nVal, int64_t nParam, int32_t nTime, int64_t LeftVal, int32_t FuLiLevel)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeUTF8(packet, name);
    Answer::NetPacket::writeInt32(packet, nType);
    Answer::NetPacket::writeInt32(packet, opWay);
    Answer::NetPacket::writeInt32(packet, nVal);
    Answer::NetPacket::writeInt64(packet, nParam);
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt64(packet, LeftVal);
    Answer::NetPacket::writeInt32(packet, FuLiLevel);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EEBu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onDBPlayerLoaded(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    PlayerDBData dbData; // [rsp+20h] [rbp-B930h] BYREF
    int32_t reason; // [rsp+B93Ch] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    reason = Answer::NetPacket::readInt32(inPacket);
    PlayerDBData::PlayerDBData(&dbData);
    PlayerDBData::UnPackageData(&dbData, inPacket, 0);
    v3 = Answer::Singleton<GameService>::instance();
    GameService::onPlayerLoaded(v3, &dbData, reason, 0);
    PlayerDBData::~PlayerDBData(&dbData);
    }
    }
}

void DBService::onDBPlayerSaved(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    const char *v3; // rax
    int8_t Id; // bl

    std::string name; // [rsp+30h] [rbp-40h] BYREF
    CharId_t cid; // [rsp+38h] [rbp-38h]
    int32_t ticks; // [rsp+44h] [rbp-2Ch]
    int64_t uid; // [rsp+48h] [rbp-28h]
    int32_t sid; // [rsp+54h] [rbp-1Ch]
    int32_t reason; // [rsp+58h] [rbp-18h]
    int32_t param; // [rsp+5Ch] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    cid = Answer::NetPacket::readUInt64(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&name, (bool)inPacket);
    ticks = Answer::NetPacket::readInt32(inPacket);
    uid = Answer::NetPacket::readInt64(inPacket);
    sid = Answer::NetPacket::readInt32(inPacket);
    reason = Answer::NetPacket::readInt32(inPacket);
    param = Answer::NetPacket::readInt32(inPacket);
    v3 = (const char *)std::string::c_str(&name);
    Answer::Logger::print(
    Answer::LogLevel::LOG_LEVEL_DEBUG,
    "use %d ticks to save player cid = %lld, name = %s\n",
    ticks,
    cid,
    v3);
    Id = MutiConn<DBService>::GetId(pConn);
    v5 = Answer::Singleton<GameService>::instance();
    GameService::onPlayerSaved(v5, Id, uid, sid, reason, param, cid);
    std::string::~string(&name);
    }
    }
}

void DBService::onDBEquipUpdated(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    MemEquip equip; // [rsp+20h] [rbp-70h] BYREF
    MemEquipVector vEquip; // [rsp+60h] [rbp-30h] BYREF
    int32_t count; // [rsp+78h] [rbp-18h]
    int32_t i; // [rsp+7Ch] [rbp-14h]

    if ( pConn && inPacket )
    {
    vEquip.vector();
    MemEquip::MemEquip(&equip);
    count = Answer::NetPacket::readInt32(inPacket);
    vEquip.reserve(count);
    for ( i = 0; i < count; ++i )
    {
    MemEquip::CleanUp(&equip);
    MemEquip::UnPackageData(&equip, inPacket);
    vEquip.push_back(&equip);
    }
    v3 = Answer::Singleton<CEquipManager>::instance();
    CEquipManager::OnEquipUpdated(v3, &vEquip);
    MemEquip::~MemEquip(&equip);
    std::vector<MemEquip>::~vector(&vEquip);
    }
}

void DBService::onDBUserPayed(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    if ( pConn )
    {
    if ( inPacket )
    {
    v3 = Answer::Singleton<GameService>::instance();
    GameService::onUserPayed(v3, inPacket);
    }
    }
}

void DBService::logShangCheng(int8_t connid, CharId_t cid, const std::string *const name, int32_t Type, int32_t ShopId, int32_t ItemClass, int32_t ItemId, int32_t ItemCount, int32_t CostGold, int64_t LeftGold, int32_t Time)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeUTF8(packet, name);
    Answer::NetPacket::writeInt32(packet, Type);
    Answer::NetPacket::writeInt32(packet, ShopId);
    Answer::NetPacket::writeInt32(packet, ItemClass);
    Answer::NetPacket::writeInt32(packet, ItemId);
    Answer::NetPacket::writeInt32(packet, ItemCount);
    Answer::NetPacket::writeInt32(packet, CostGold);
    Answer::NetPacket::writeInt64(packet, LeftGold);
    Answer::NetPacket::writeInt32(packet, Time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EE9u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::OnSendMail(int8_t connid, const MailInfo *const Mail, const std::string *const Param, FamilyId_t nFamilyid)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-18h]
    int8_t Count; // [rsp+47h] [rbp-9h]
    int32_t Offset; // [rsp+48h] [rbp-8h]
    char i; // [rsp+4Fh] [rbp-1h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, Mail->MailId);
    Answer::NetPacket::writeInt32(packet, Mail->SysMailId);
    Answer::NetPacket::writeInt64(packet, Mail->SenderId);
    Answer::NetPacket::writeUTF8(packet, &Mail->SenderName);
    Answer::NetPacket::writeInt64(packet, Mail->ReceiveId);
    Answer::NetPacket::writeUTF8(packet, &Mail->ReceiveName);
    Answer::NetPacket::writeInt32(packet, Mail->SendTime);
    Answer::NetPacket::writeInt8(packet, Mail->HasRead);
    Answer::NetPacket::writeInt8(packet, Mail->Extract);
    Answer::NetPacket::writeUTF8(packet, &Mail->MailTitle);
    Answer::NetPacket::writeUTF8(packet, &Mail->MailContent);
    Answer::NetPacket::writeInt32(packet, Mail->nReason);
    Answer::NetPacket::writeUTF8(packet, &Mail->Param);
    if ( Mail->Extract == 1 )
    {
    Count = 0;
    Offset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::writeInt8(packet, 0);
    for ( i = 0; i <= 5; ++i )
    {
    if ( Mail->Item[i].itemId > 0 && Mail->Item[i].itemCount > 0 )
    {
    Answer::NetPacket::writeInt8(packet, i);
    Answer::NetPacket::writeInt32(packet, Mail->Item[i].itemId);
    Answer::NetPacket::writeInt8(packet, Mail->Item[i].itemClass);
    Answer::NetPacket::writeInt32(packet, Mail->Item[i].itemCount);
    Answer::NetPacket::writeInt8(packet, Mail->Item[i].bind);
    Answer::NetPacket::writeInt32(packet, Mail->Item[i].endTime);
    Answer::NetPacket::writeInt64(packet, Mail->Item[i].srcId);
    ++Count;
    }
    }
    Answer::NetPacket::getBuffer(packet)[Offset] = Count;
    }
    Answer::NetPacket::writeInt64(packet, nFamilyid);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EECu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::OnDeleteMail(int8_t connid, int32_t nMailId)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nMailId);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EEDu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::OnUpdateMailInfo(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    v3 = Answer::Singleton<GameService>::instance();
    GameService::OnUpdateMail(v3, inPacket);
}

void DBService::OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const std::string *const Param, FamilyId_t nFamilyid)
{

    const char *v7; // rax
    const char *v9; // rax
    const char *v11; // rax

    MailInfo Mail; // [rsp+30h] [rbp-130h] BYREF
    CfgSysMail *pSysMail; // [rsp+140h] [rbp-20h]
    int32_t FuJianCount; // [rsp+148h] [rbp-18h]
    int32_t i; // [rsp+14Ch] [rbp-14h]

    memset(&Mail, 0, 268);
    std::string::string(&Mail.SenderName);
    std::string::string(&Mail.ReceiveName);
    std::string::string(&Mail.MailTitle);
    std::string::string(&Mail.MailContent);
    std::string::string(&Mail.Param);
    v6 = Answer::Singleton<CfgData>::instance();
    pSysMail = CfgData::GetSysMail(v6, SysMailId);
    if ( pSysMail )
    {
    if ( !(unsigned __int8)std::string::empty(&pSysMail->sender_name) )
    {
    v7 = (const char *)std::string::c_str(&pSysMail->sender_name);
    if ( Answer::StringUtility::utf8Strlen(v7) <= 30 && !(unsigned __int8)std::string::empty(&pSysMail->title) )
    {
    v9 = (const char *)std::string::c_str(&pSysMail->title);
    if ( Answer::StringUtility::utf8Strlen(v9) <= 30 && !(unsigned __int8)std::string::empty(&pSysMail->content) )
    {
    v11 = (const char *)std::string::c_str(&pSysMail->content);
    if ( Answer::StringUtility::utf8Strlen(v11) <= 500 )
    {
    Mail.SysMailId = SysMailId;
    Mail.ReceiveId = ReceiverId;
    Mail.SendTime = Answer::DayTime::now();
    std::string::operator=(&Mail.Param, Param);
    Mail.nReason = 2;
    FuJianCount = pSysMail->item.size();
    if ( FuJianCount > 0 && FuJianCount <= 5 )
    {
    Mail.Extract = 1;
    for ( i = 0; i < FuJianCount; ++i )
    {
    v13 = i;
    Mail.Item[v13] = *std::vector<MemChrBag>::operator[](&pSysMail->item, i);
    }
    }
    this->OnSendMail(connid, &Mail, Param, nFamilyid);
    }
    }
    }
    }
    }
    MailInfo::~MailInfo(&Mail);
}

void DBService::OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const MemChrBag *const item, ITEM_CHANGE_REASON nReason, const std::string *const Param, FamilyId_t nFamilyid)
{
    MemChrBagVector items; // [rsp+40h] [rbp-30h] BYREF

    items.vector();
    if ( item->itemId > 0 && item->itemCount > 0 )
    items.push_back(item);
    this->OnSendSysMail(connid, ReceiverId, SysMailId, &items, nReason, Param, nFamilyid);
    std::vector<MemChrBag>::~vector(&items);
}

void DBService::OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const MemChrBagVector *const items, ITEM_CHANGE_REASON nReason, const std::string *const Param, FamilyId_t nFamilyid)
{

    const char *v9; // rax
    const char *v11; // rax
    const char *v13; // rax

    MailInfo Mail; // [rsp+30h] [rbp-130h] BYREF
    CfgSysMail *pSysMail; // [rsp+140h] [rbp-20h]
    int32_t FuJianCount; // [rsp+148h] [rbp-18h]
    int32_t i; // [rsp+14Ch] [rbp-14h]

    memset(&Mail, 0, 268);
    std::string::string(&Mail.SenderName);
    std::string::string(&Mail.ReceiveName);
    std::string::string(&Mail.MailTitle);
    std::string::string(&Mail.MailContent);
    std::string::string(&Mail.Param);
    v8 = Answer::Singleton<CfgData>::instance();
    pSysMail = CfgData::GetSysMail(v8, SysMailId);
    if ( pSysMail )
    {
    if ( !(unsigned __int8)std::string::empty(&pSysMail->sender_name) )
    {
    v9 = (const char *)std::string::c_str(&pSysMail->sender_name);
    if ( Answer::StringUtility::utf8Strlen(v9) <= 30 && !(unsigned __int8)std::string::empty(&pSysMail->title) )
    {
    v11 = (const char *)std::string::c_str(&pSysMail->title);
    if ( Answer::StringUtility::utf8Strlen(v11) <= 30 && !(unsigned __int8)std::string::empty(&pSysMail->content) )
    {
    v13 = (const char *)std::string::c_str(&pSysMail->content);
    if ( Answer::StringUtility::utf8Strlen(v13) <= 500 )
    {
    Mail.SysMailId = SysMailId;
    Mail.ReceiveId = ReceiverId;
    Mail.SendTime = Answer::DayTime::now();
    std::string::operator=(&Mail.Param, Param);
    Mail.nReason = nReason;
    FuJianCount = std::vector<MemChrBag>::size(items);
    if ( FuJianCount > 0 && FuJianCount <= 5 )
    {
    Mail.Extract = 1;
    for ( i = 0; i < FuJianCount; ++i )
    {
    v15 = i;
    Mail.Item[v15] = *std::vector<MemChrBag>::operator[](items, i);
    }
    }
    this->OnSendMail(connid, &Mail, Param, nFamilyid);
    }
    }
    }
    }
    }
    MailInfo::~MailInfo(&Mail);
}

void DBService::onNewMinuteCome(int32_t minute)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, minute);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EEFu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::OnUpdatePay()
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt8(packet, 0);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EF0u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveCityWarResult(int32_t nActId, FamilyId_t nFamilyId, int32_t nWinTime, int32_t nTime, CharId_t nLeaderCid, CharId_t First, CharId_t Second, CharId_t Third, int32_t nIndex, std::string *p_FirstFamilyName, std::string *p_SecondFamilyName, std::string *p_ThirdFamilyName)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nActId);
    Answer::NetPacket::writeInt64(packet, nFamilyId);
    Answer::NetPacket::writeInt32(packet, nWinTime);
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt64(packet, nLeaderCid);
    Answer::NetPacket::writeInt64(packet, First);
    Answer::NetPacket::writeInt64(packet, Second);
    Answer::NetPacket::writeInt64(packet, Third);
    Answer::NetPacket::writeInt32(packet, nIndex);
    Answer::NetPacket::writeUTF8(packet, p_FirstFamilyName);
    Answer::NetPacket::writeUTF8(packet, p_SecondFamilyName);
    Answer::NetPacket::writeUTF8(packet, p_ThirdFamilyName);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EF1u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveCrossTowerResult(int32_t nActId, CharId_t Winner, std::string *p_name, int32_t battle, int8_t connid, int32_t time)
{
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+30h] [rbp-50h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+40h] [rbp-40h] BYREF
    std::string val; // [rsp+50h] [rbp-30h] BYREF

    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+60h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+68h] [rbp-18h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( !packet )
    return;
    if ( MutiConn<DBService>::GetId(pConn) == connid )
    {
    Answer::NetPacket::writeInt32(packet, nActId);
    Answer::NetPacket::writeInt64(packet, Winner);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::writeInt32(packet, battle);
    }
    else
    {
    Answer::NetPacket::writeInt32(packet, nActId);
    Answer::NetPacket::writeInt64(packet, 0);

    std::string::string(&val, byte_8CFE00, &v15);
    Answer::NetPacket::writeUTF8(packet, &val);
    std::string::~string(&val);
    std::allocator<char>::~allocator(&v15);
    Answer::NetPacket::writeInt32(packet, 0);
    }
    Answer::NetPacket::writeInt32(packet, time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EF2u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdateCityWarResult(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    int8_t Id; // bl

    std::string ThirdFamilyName; // [rsp+60h] [rbp-B0h] BYREF
    std::string SecondFamilyName; // [rsp+70h] [rbp-A0h] BYREF
    std::string FirstFamilyName; // [rsp+80h] [rbp-90h] BYREF
    std::string p_ThirdFamilyName; // [rsp+90h] [rbp-80h] BYREF
    std::string p_SecondFamilyName; // [rsp+A0h] [rbp-70h] BYREF
    std::string p_FirstFamilyName; // [rsp+B0h] [rbp-60h] BYREF
    int32_t nActId; // [rsp+C4h] [rbp-4Ch]
    FamilyId_t nFamilyId; // [rsp+C8h] [rbp-48h]
    int32_t nWinTime; // [rsp+D0h] [rbp-40h]
    int32_t nTime; // [rsp+D4h] [rbp-3Ch]
    CharId_t nLeaderCid; // [rsp+D8h] [rbp-38h]
    CharId_t First; // [rsp+E0h] [rbp-30h]
    CharId_t Second; // [rsp+E8h] [rbp-28h]
    CharId_t Third; // [rsp+F0h] [rbp-20h]
    int32_t nIndex; // [rsp+FCh] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nActId = Answer::NetPacket::readInt32(inPacket);
    nFamilyId = Answer::NetPacket::readInt64(inPacket);
    nWinTime = Answer::NetPacket::readInt32(inPacket);
    nTime = Answer::NetPacket::readInt32(inPacket);
    nLeaderCid = Answer::NetPacket::readInt64(inPacket);
    First = Answer::NetPacket::readInt64(inPacket);
    Second = Answer::NetPacket::readInt64(inPacket);
    Third = Answer::NetPacket::readInt64(inPacket);
    nIndex = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&FirstFamilyName, (bool)inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&SecondFamilyName, (bool)inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&ThirdFamilyName, (bool)inPacket);
    std::string::string(&p_ThirdFamilyName, &ThirdFamilyName);
    std::string::string(&p_SecondFamilyName, &SecondFamilyName);
    std::string::string(&p_FirstFamilyName, &FirstFamilyName);
    Id = MutiConn<DBService>::GetId(pConn);
    v4 = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::OnCityWarResult(
    v4,
    Id,
    nActId,
    nFamilyId,
    nWinTime,
    nTime,
    nLeaderCid,
    First,
    Second,
    Third,
    nIndex,
    &p_FirstFamilyName,
    &p_SecondFamilyName,
    &p_ThirdFamilyName);
    std::string::~string(&p_FirstFamilyName);
    std::string::~string(&p_SecondFamilyName);
    std::string::~string(&p_ThirdFamilyName);
    std::string::~string(&ThirdFamilyName);
    std::string::~string(&SecondFamilyName);
    std::string::~string(&FirstFamilyName);
    }
    }
}

void DBService::onUpdateCrossTowerResutl(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    int8_t Id; // bl

    std::string Name; // [rsp+20h] [rbp-40h] BYREF
    std::string p_Name; // [rsp+30h] [rbp-30h] BYREF
    int32_t nActId; // [rsp+3Ch] [rbp-24h]
    CharId_t Winner; // [rsp+40h] [rbp-20h]
    int32_t Battle; // [rsp+4Ch] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nActId = Answer::NetPacket::readInt32(inPacket);
    Winner = Answer::NetPacket::readInt64(inPacket);
    Battle = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&Name, (bool)inPacket);
    std::string::string(&p_Name, &Name);
    Id = MutiConn<DBService>::GetId(pConn);
    v4 = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::OnCrossTower(v4, Id, nActId, Winner, &p_Name, Battle);
    std::string::~string(&p_Name);
    std::string::~string(&Name);
    }
    }
}

void DBService::SavePeerlessWarResult(int32_t nActId, CharId_t winner, std::string *p_name, int32_t nTime)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nActId);
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt64(packet, winner);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EFDu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdatePeerlessWarResult(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    int8_t Id; // bl

    std::string name; // [rsp+20h] [rbp-30h] BYREF
    int32_t nActId; // [rsp+34h] [rbp-1Ch]
    CharId_t winner; // [rsp+38h] [rbp-18h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nActId = Answer::NetPacket::readInt32(inPacket);
    winner = Answer::NetPacket::readInt64(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&name, (bool)inPacket);
    Id = MutiConn<DBService>::GetId(pConn);
    v4 = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::OnPeerlessWarResult(v4, Id, nActId, winner, &name);
    std::string::~string(&name);
    }
    }
}

void DBService::onUpdateBossFirstKilled(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    std::string strName; // [rsp+20h] [rbp-40h] BYREF
    std::string p_Name; // [rsp+30h] [rbp-30h] BYREF
    CharId_t nCharId; // [rsp+40h] [rbp-20h]
    int32_t BossId; // [rsp+48h] [rbp-18h]
    int32_t time; // [rsp+4Ch] [rbp-14h]

    if ( pConn )
    {
    if ( inPacket )
    {
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&strName, (bool)inPacket);
    nCharId = Answer::NetPacket::readInt64(inPacket);
    BossId = Answer::NetPacket::readInt32(inPacket);
    time = Answer::NetPacket::readInt32(inPacket);
    std::string::string(&p_Name, &strName);
    v3 = Answer::Singleton<CKaiFuHuoDong>::instance();
    CKaiFuHuoDong::OnUpdateKilledMonster(v3, BossId, nCharId, &p_Name, time);
    std::string::~string(&p_Name);
    std::string::~string(&strName);
    }
    }
}

void DBService::onUpdateKaiFuTime(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    int32_t kaiFuTime; // [rsp+2Ch] [rbp-4h]

    if ( pConn )
    {
    if ( inPacket )
    {
    kaiFuTime = Answer::NetPacket::readInt32(inPacket);
    v3 = Answer::Singleton<CfgData>::instance();
    CfgData::updateServerStartTime(v3, kaiFuTime);
    }
    }
}

void DBService::onUpdateGMBroadcast(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    v3 = Answer::Singleton<CGMBackstate>::instance();
    CGMBackstate::onUpdateGMBroadcast(v3, inPacket);
}

void DBService::onUpdateGMBanChat(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    int8_t Id; // bl

    uint32_t Size; // ebx
    char *Buffer; // rcx
    uint32_t WOffset; // edx

    Answer::NetPacket *packet; // [rsp+28h] [rbp-18h]

    v3 = Answer::Singleton<CGMBackstate>::instance();
    CGMBackstate::onUpdateGMBanChat(v3, inPacket);
    v4 = Answer::Singleton<GameService>::instance();
    if ( GameService::getLine(v4) == 1 )
    {
    Id = MutiConn<DBService>::GetId(pConn);
    v6 = Answer::Singleton<GameService>::instance();
    packet = GameService::popNetpacket(v6, Id, Answer::PackType::PACK_DISPATCH, 0x4E4Eu);
    if ( packet )
    {
    Size = Answer::NetPacket::getSize(inPacket);
    Buffer = Answer::NetPacket::getBuffer(inPacket);
    Answer::NetPacket::write(packet, Buffer, Size);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    LOBYTE(Size) = MutiConn<DBService>::GetId(pConn);
    v10 = Answer::Singleton<GameService>::instance();
    GameService::sendPacket(v10, Size, packet);
    }
    }
}

void DBService::onUpdateGMSeal(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


    v3 = Answer::Singleton<CGMBackstate>::instance();
    CGMBackstate::onUpdateGMSeal(v3, inPacket);
}

void DBService::onLoadGMBackstage()
{
    int8_t Id; // al
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Id = MutiConn<DBService>::GetId(pConn);
    Answer::NetPacket::writeInt8(packet, Id);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EFBu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
    }
}

void DBService::onBackStageKick(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    CharId_t nCharId; // [rsp+28h] [rbp-8h]

    if ( pConn && inPacket )
    {
    nCharId = Answer::NetPacket::readInt64(inPacket);
    v3 = Answer::Singleton<GameService>::instance();
    if ( nCharId <= 0 )
    GameService::KickAll(v3, 1);
    else
    GameService::KickUser(v3, nCharId, 1);
    }
}

void DBService::ReloadTencentInfo(int8_t connid, CharId_t cid)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F1Eu);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onUpdateTencentInfo(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    TencentInfo info; // [rsp+20h] [rbp-40h] BYREF
    CharId_t cid; // [rsp+48h] [rbp-18h]

    if ( pConn )
    {
    if ( inPacket )
    {
    cid = Answer::NetPacket::readInt64(inPacket);
    TencentInfo::TencentInfo(&info);
    TencentInfo::UnPacketInfo(&info, inPacket);
    v3 = Answer::Singleton<GameService>::instance();
    GameService::UpdateTencentInfo(v3, cid, &info);
    TencentInfo::~TencentInfo(&info);
    }
    }
}

void DBService::CheckMobilePhoneGiftEffect(int8_t connid, int16_t cgindex, int32_t sid, std::string *p_passport, int8_t nType)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeUTF8(packet, p_passport);
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F20u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::CheckGuardPrivilege(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int8_t type, int32_t param, int8_t index)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-20h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-18h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt32(packet, param);
    Answer::NetPacket::writeInt8(packet, index);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F22u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::OnDBClickGame(int8_t connid, int64_t uid, int32_t sid, int32_t start_time)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt32(packet, start_time);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F24u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::onLoadCityWarInfo()
{
    int8_t Id; // al
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Id = MutiConn<DBService>::GetId(pConn);
    Answer::NetPacket::writeInt8(packet, Id);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ED2u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
    }
}

void DBService::onCityWarInfoLoaded(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    int8_t Id; // bl

    int64_t nFamilyId; // [rsp+28h] [rbp-18h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nFamilyId = Answer::NetPacket::readInt64(inPacket);
    Id = MutiConn<DBService>::GetId(pConn);
    v4 = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetCityWarWinner(v4, Id, nFamilyId);
    }
    }
}

void DBService::onLoadPeerlessWarInfo()
{
    int8_t Id; // al
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Id = MutiConn<DBService>::GetId(pConn);
    Answer::NetPacket::writeInt8(packet, Id);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ED4u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
    }
}

void DBService::onPeerlessWarInfoLoaded(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    int8_t Id; // bl

    int64_t nCharId; // [rsp+28h] [rbp-18h]

    if ( pConn )
    {
    if ( inPacket )
    {
    nCharId = Answer::NetPacket::readInt64(inPacket);
    Id = MutiConn<DBService>::GetId(pConn);
    v4 = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetPeerlessWarWinner(v4, Id, nCharId);
    }
    }
}

void DBService::onLoadCrossTowerInfo()
{
    int8_t Id; // al
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Id = MutiConn<DBService>::GetId(pConn);
    Answer::NetPacket::writeInt8(packet, Id);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ED5u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
    }
}

void DBService::onLoadServerBattle()
{
    int8_t Id; // al
    uint32_t WOffset; // eax
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > iter; // [rsp+10h] [rbp-30h] BYREF
    std::_Rb_tree_iterator<std::pair<const signed char,DBService::ConnType*> > __x; // [rsp+20h] [rbp-20h] BYREF
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+30h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+38h] [rbp-8h]

    for ( iter = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::begin(&this->m_mConn);
    ;
    std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator++(&iter) )
    {
    __x = std::map<signed char,MutiConn<DBService> *,std::less<signed char>,std::allocator<std::pair<signed char const,MutiConn<DBService> *>>>::end(&this->m_mConn);
    if ( !std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator!=(&iter, &__x) )
    break;
    pConn = std::_Rb_tree_iterator<std::pair<signed char const,MutiConn<DBService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Id = MutiConn<DBService>::GetId(pConn);
    Answer::NetPacket::writeInt8(packet, Id);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4ED6u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
    }
}

void DBService::onCrossTowerInfoLoaded(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    std::string sName; // [rsp+20h] [rbp-40h] BYREF
    std::string p_Name; // [rsp+30h] [rbp-30h] BYREF
    CharId_t nCharId; // [rsp+40h] [rbp-20h]
    int32_t nBattle; // [rsp+4Ch] [rbp-14h]

    if ( pConn && inPacket )
    {
    nCharId = Answer::NetPacket::readInt64(inPacket);
    nBattle = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&sName, (bool)inPacket);
    if ( nCharId > 0 )
    {
    std::string::string(&p_Name, &sName);
    v3 = Answer::Singleton<CActivityManager>::instance();
    CActivityManager::SetCrossTowerWinner(v3, nCharId, nBattle, &p_Name);
    std::string::~string(&p_Name);
    }
    std::string::~string(&sName);
    }
}

void DBService::onServerBattleLoaded(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

    int Battle; // [rsp+2Ch] [rbp-4h]

    if ( pConn && inPacket )
    {
    Battle = Answer::NetPacket::readInt32(inPacket);
    if ( Battle > 0 )
    {
    v3 = Answer::Singleton<GameService>::instance();
    GameService::SetServerBattle(v3, Battle);
    }
    }
}

void DBService::onPlayerNetPacket(std::map<signed char,DBService::ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
    int16_t Int16; // r12
    int8_t Id; // bl


    if ( pConn )
    {
    if ( inPacket )
    {
    Int16 = Answer::NetPacket::readInt16(inPacket);
    Id = MutiConn<DBService>::GetId(pConn);
    v5 = Answer::Singleton<GameService>::instance();
    GameService::OnDBNetPacket(v5, Id, Int16, inPacket);
    }
    }
}

void DBService::SaveEquipBackRecord(int32_t nId, int8_t nType, int32_t Time, std::string *p_name)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nId);
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt32(packet, Time);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F01u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveEquipBackCount(int32_t nId, int8_t nType, int32_t Count, std::string *p_name)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, nId);
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F02u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveGuiGuDaoRenData(int32_t NpcId, int32_t Count)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, NpcId);
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F03u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveBackEquipCount(int32_t Count)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F04u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::SaveBackEQuipRank(EquipBackRankCfg *p_stu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    EquipBackRankCfg::PackageData(p_stu, packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F05u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::logAbnormal(log_abnormal *p_stu)
{
    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+10h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+18h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    log_abnormal::PackageData(p_stu, packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F06u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::LogXinMoBag(CharId_t cid, MemChrBag item, int32_t falg, int32_t Opway)
{

    uint32_t WOffset; // edx
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    pConn = this->GetConn(0);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, item.itemId);
    Answer::NetPacket::writeInt8(packet, item.itemClass);
    Answer::NetPacket::writeInt32(packet, item.itemCount);
    Answer::NetPacket::writeInt64(packet, item.srcId);
    Answer::NetPacket::writeInt32(packet, falg);
    Answer::NetPacket::writeInt32(packet, Opway);
    v5 = Answer::DayTime::now();
    Answer::NetPacket::writeInt32(packet, v5);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F07u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
}

void DBService::AddPlatformLog(int8_t connid, PLATFORM_LOG_DATA_TYPE nType, platform_log_base *pData)
{
    uint32_t WOffset; // edx
    int8_t nTypea; // [rsp+10h] [rbp-20h]
    std::map<signed char,DBService::ConnType*>::ConnType *pConn; // [rsp+20h] [rbp-10h]
    Answer::NetPacket *packet; // [rsp+28h] [rbp-8h]

    nTypea = nType;
    if ( pData )
    {
    pConn = this->GetConn(connid);
    if ( pConn )
    {
    packet = Answer::TcpClient::popNetpacket(pConn);
    if ( packet )
    {
    Answer::NetPacket::writeInt8(packet, nTypea);
    /* vcall: (*((void (__fastcall **)(platform_log_base *, Answer::NetPacket *))pData->_vptr_platform_log_base + 4))( */
    pData,
    packet);
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4F25u);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    }
    }
    }
}

（内容由AI生成，仅供参考）
