#include "Game/DBService.h"
#include "Game/GameService.h"
#include "Game/CActivityManager.h"
#include "Other/CEquipBackRecord.h"
#include "Other/CEquipBackInfo.h"
#include "Config/EquipBackRankCfg.h"
#include <cstring>
#include <algorithm>

// Note: ConnType is MutiConn<DBService>

DBService::DBService()
{
    std::map<signed char, MutiConn<DBService> *>::map(&this->m_mConn);
}

DBService::~DBService()
{
    std::map<signed char, MutiConn<DBService> *>::~map(&this->m_mConn);
}

void DBService::Init(int32_t line)
{
    if (line == 9)
        DBService::onLoadGMBackstage(this);
}

bool DBService::Connect(int8_t id, std::string *p_host, int32_t port)
{
    uint16_t porta = (uint16_t)port;
    int8_t ida[4];
    ida[0] = id;
    
    std::string name("DBService");
    ConnType *v4 = (ConnType *)operator new(0x118u);
    MutiConn<DBService>::MutiConn(v4, 0x2000, 0x2000, 0x3FF9u, 0, 0x7D0u, &name, id, this);
    ConnType *pConn = v4;
    name.~basic_string();
    
    if (!pConn)
        return false;
    
    Answer::InetAddress serverAddr;
    Answer::InetAddress::InetAddress(&serverAddr, p_host, porta);
    bool v6 = !Answer::TcpClient::connect(pConn, &serverAddr);
    Answer::InetAddress::~InetAddress(&serverAddr);
    if (v6)
        return false;
    
    Answer::Thread::start(pConn);
    ConnType **v7 = std::map<signed char, MutiConn<DBService> *>::operator[](&this->m_mConn, ida);
    *v7 = pConn;
    
    Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
    if (!packet)
        return false;
    
    std::string val;
    Answer::TcpClient::getName(pConn, &val);
    Answer::NetPacket::writeUTF8(packet, &val);
    val.~basic_string();
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setProc(packet, 0x4EC6u);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    Answer::TcpClient::sendPacket(pConn, packet);
    return true;
}

ConnType *DBService::GetConn(int8_t connid)
{
    auto iter = this->m_mConn.find(connid);
    if (iter != this->m_mConn.end())
        return iter->second;
    return nullptr;
}

void DBService::onNetPacket(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        switch (Answer::NetPacket::getProc(inPacket))
        {
            case 0x4EC9: onDBPlayerLoaded(pConn, inPacket); break;
            case 0x4ECC: onDBPlayerSaved(pConn, inPacket); break;
            case 0x4ED0: onDBEquipUpdated(pConn, inPacket); break;
            case 0x4ED1: onDBUserPayed(pConn, inPacket); break;
            case 0x4ED3: onCityWarInfoLoaded(pConn, inPacket); break;
            case 0x4ED7: onPeerlessWarInfoLoaded(pConn, inPacket); break;
            case 0x4ED8: onCrossTowerInfoLoaded(pConn, inPacket); break;
            case 0x4ED9: onServerBattleLoaded(pConn, inPacket); break;
            case 0x4EDE: onUpdateWorldBossInfo(pConn, inPacket); break;
            case 0x4EEE: onUpdateEquipBackRecord(pConn, inPacket); break;
            case 0x4EF3: onUpdateEquipBackInfo(pConn, inPacket); break;
            case 0x4EF4: onUpdateDoubleElevenRank(pConn, inPacket); break;
            case 0x4EF7: onUpdateZHYYHDRank(pConn, inPacket); break;
            case 0x4EF8: onUpdateDropTimes(pConn, inPacket); break;
            case 0x4EF9: onUpdateMYSJReward(pConn, inPacket); break;
            case 0x4EFA: onUpdateCityWarResult(pConn, inPacket); break;
            case 0x4EFC: onUpdateCrossTowerResutl(pConn, inPacket); break;
            case 0x4EFE: onUpdatePeerlessWarResult(pConn, inPacket); break;
            case 0x4EFF: onUpdateBossFirstKilled(pConn, inPacket); break;
            case 0x4F08: onUpdateKaiFuTime(pConn, inPacket); break;
            case 0x4F09: onUpdateGMBroadcast(pConn, inPacket); break;
            case 0x4F0A: onUpdateGMBanChat(pConn, inPacket); break;
            case 0x4F0D: onUpdateGMSeal(pConn, inPacket); break;
            case 0x4F10: onUpdateTencentInfo(pConn, inPacket); break;
            case 0x4F13: onUpdatePay(pConn, inPacket); break;
            case 0x4F1F: break; // unknown
            default: onPlayerNetPacket(pConn, inPacket); break;
        }
        Answer::NetPacket::destroy(inPacket);
    }
}

void DBService::fengHao(int8_t connid, int8_t Type, int64_t Cid, std::string *p_name)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt8(packet, Type);
            Answer::NetPacket::writeInt64(packet, Cid);
            Answer::NetPacket::writeUTF8(packet, p_name);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EBFu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::UpdateLoginTime(int8_t connid, CharId_t cid, int32_t login_time)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, login_time);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC1u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogPlayerStay(int8_t connid, int32_t nTime, int32_t nDay)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nTime);
            Answer::NetPacket::writeInt32(packet, nDay);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC2u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::insertTaskInfo(int8_t connid, const LogTask *const task)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, task->nCharId);
            Answer::NetPacket::writeInt32(packet, task->nTaskId);
            Answer::NetPacket::writeInt8(packet, task->nState);
            Answer::NetPacket::writeInt32(packet, task->nProgress);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC3u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::updateTaskInfo(int8_t connid, const LogTask *const task)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, task->nCharId);
            Answer::NetPacket::writeInt32(packet, task->nTaskId);
            Answer::NetPacket::writeInt8(packet, task->nState);
            Answer::NetPacket::writeInt32(packet, task->nProgress);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC4u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::insertDungeon(int8_t connid, const LogDungeon *const dungeon)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, dungeon->nCharId);
            Answer::NetPacket::writeInt32(packet, dungeon->nDungeonId);
            Answer::NetPacket::writeInt8(packet, dungeon->nResult);
            Answer::NetPacket::writeInt32(packet, dungeon->nScore);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC5u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::updateDungeon(int8_t connid, const LogDungeon *const dungeon)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, dungeon->nCharId);
            Answer::NetPacket::writeInt32(packet, dungeon->nDungeonId);
            Answer::NetPacket::writeInt8(packet, dungeon->nResult);
            Answer::NetPacket::writeInt32(packet, dungeon->nScore);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC6u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveWorldBossInfo(const WorldBossInfo *const info)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, info->nBossId);
            Answer::NetPacket::writeInt8(packet, info->nState);
            Answer::NetPacket::writeInt64(packet, info->nHp);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC7u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveDropTimes(int32_t nGroupId, int32_t nTimes)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nGroupId);
            Answer::NetPacket::writeInt32(packet, nTimes);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC8u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::ClearDropTimes()
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EC9u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveDropRecord(const DropRecord *const record)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, record->nGroupId);
            Answer::NetPacket::writeInt32(packet, record->nItemId);
            Answer::NetPacket::writeInt32(packet, record->nCount);
            Answer::NetPacket::writeInt64(packet, record->nCharId);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ECAu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::savePlayer(int8_t connid, Player *player, int32_t reason, int32_t param, int32_t logout_time)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            // Write player data
            Answer::NetPacket::writeInt64(packet, player->getCid());
            Answer::NetPacket::writeInt32(packet, reason);
            Answer::NetPacket::writeInt32(packet, param);
            Answer::NetPacket::writeInt32(packet, logout_time);
            // Write character data
            Player::SaveDBData(player, packet);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ECBu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::loadPlayer(int8_t connid, int64_t cid, int16_t cgindex, int32_t sid)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt16(packet, cgindex);
            Answer::NetPacket::writeInt32(packet, sid);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ECCu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::insertMemEquip(int8_t connid, MemEquip *equip, int32_t reason)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, equip->nCharId);
            MemEquip::PackageData(equip, packet);
            Answer::NetPacket::writeInt32(packet, reason);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ECDu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::updateMemEquip(int8_t connid, MemEquip *equip, int32_t reason)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, equip->nCharId);
            MemEquip::PackageData(equip, packet);
            Answer::NetPacket::writeInt32(packet, reason);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ECEu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::deleteMemEquip(int8_t connid, int64_t nCharId, int32_t nEquipId, int32_t reason)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, nCharId);
            Answer::NetPacket::writeInt32(packet, nEquipId);
            Answer::NetPacket::writeInt32(packet, reason);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ECFu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::InsertBossLog(int8_t connid, const LogBoss *const boss)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, boss->nCharId);
            Answer::NetPacket::writeInt32(packet, boss->nBossId);
            Answer::NetPacket::writeInt32(packet, boss->nDamage);
            Answer::NetPacket::writeInt32(packet, boss->nTime);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED0u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::InsertActivityLog(int8_t connid, const LogActivity *const activity)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, activity->nCharId);
            Answer::NetPacket::writeInt32(packet, activity->nActivityId);
            Answer::NetPacket::writeInt32(packet, activity->nValue);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED1u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::InsertDailyGiftLog(int8_t connid, const LogDailyGift *const gift)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, gift->nCharId);
            Answer::NetPacket::writeInt32(packet, gift->nGiftId);
            Answer::NetPacket::writeInt8(packet, gift->nDay);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED2u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::insertItemChange(int8_t connid, const LogItemChange *const item)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, item->nCharId);
            Answer::NetPacket::writeInt32(packet, item->nItemId);
            Answer::NetPacket::writeInt32(packet, item->nChangeCount);
            Answer::NetPacket::writeInt32(packet, item->nReason);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED3u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::insertMingGeItem(int8_t connid, const MGLog *const logStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, logStu->nCharId);
            Answer::NetPacket::writeInt32(packet, logStu->nItemId);
            Answer::NetPacket::writeInt32(packet, logStu->nCount);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED4u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logLevelUp(int8_t connid, const LogPlayerLevel *const logLevel)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, logLevel->nCharId);
            Answer::NetPacket::writeInt32(packet, logLevel->nOldLevel);
            Answer::NetPacket::writeInt32(packet, logLevel->nNewLevel);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED5u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveOnlineTime(int8_t connid, int64_t cid, int32_t nOnlineTime)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, nOnlineTime);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED6u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::log360(int8_t connid, Log360 *const LogStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, LogStu->nCharId);
            Answer::NetPacket::writeInt32(packet, LogStu->nType);
            Answer::NetPacket::writeInt32(packet, LogStu->nValue);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED7u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogProc(int8_t connid, PacketProcLog *const stu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, stu->nCharId);
            Answer::NetPacket::writeInt32(packet, stu->nProc);
            Answer::NetPacket::writeInt32(packet, stu->nTime);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED8u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logGeneral(int8_t connid, Generallog *const LogStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, LogStu->nCharId);
            Answer::NetPacket::writeInt32(packet, LogStu->nType);
            Answer::NetPacket::writeInt32(packet, LogStu->nValue);
            Answer::NetPacket::writeInt32(packet, LogStu->nParam);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4ED9u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogUpEquipStar(int8_t connid, UpStartLog *const LogStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, LogStu->nCharId);
            Answer::NetPacket::writeInt32(packet, LogStu->nEquipId);
            Answer::NetPacket::writeInt32(packet, LogStu->nOldStar);
            Answer::NetPacket::writeInt32(packet, LogStu->nNewStar);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EDAu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogEquipDrop(int8_t connid, DropLog *const LogStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, LogStu->nCharId);
            Answer::NetPacket::writeInt32(packet, LogStu->nMonsterId);
            Answer::NetPacket::writeInt32(packet, LogStu->nItemId);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EDBu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogChangeMap(int8_t connid, ChangeMapLog *const LogStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, LogStu->nCharId);
            Answer::NetPacket::writeInt32(packet, LogStu->nOldMapId);
            Answer::NetPacket::writeInt32(packet, LogStu->nNewMapId);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EDCu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogNpcFunc(int8_t connid, NpcFuncLog *p_LogStu)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, p_LogStu->nCharId);
            Answer::NetPacket::writeInt32(packet, p_LogStu->nNpcId);
            Answer::NetPacket::writeInt32(packet, p_LogStu->nFuncId);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EDDu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logMoYuShiJieReward(int8_t connid, int64_t cid, int32_t nRewardId, int32_t nCount)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, nRewardId);
            Answer::NetPacket::writeInt32(packet, nCount);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EDEu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logPlayerLogin(int8_t connid, int64_t cid, int32_t nLoginTime)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, nLoginTime);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EDFu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logCurrency(int8_t connid, int64_t cid, int32_t nType, int32_t nChange, int32_t nReason)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, nType);
            Answer::NetPacket::writeInt32(packet, nChange);
            Answer::NetPacket::writeInt32(packet, nReason);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EE0u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::onDBPlayerLoaded(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int64_t cid = Answer::NetPacket::readInt64(inPacket);
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::onDBPlayerLoaded(v4, Id, cid, inPacket);
    }
}

void DBService::onDBPlayerSaved(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int64_t cid = Answer::NetPacket::readInt64(inPacket);
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::onDBPlayerSaved(v4, Id, cid, inPacket);
    }
}

void DBService::onDBEquipUpdated(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::onDBEquipUpdated(v4, Id, inPacket);
    }
}

void DBService::onDBUserPayed(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::onDBUserPayed(v4, Id, inPacket);
    }
}

void DBService::LoadDoubleElevenRank(int8_t nType, int8_t nDay)
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                Answer::NetPacket::writeInt8(packet, nType);
                Answer::NetPacket::writeInt8(packet, nDay);
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4EE1u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::SaveDoubleElevenRank(int8_t connid, int32_t nRankType, int32_t nCharId, int32_t nValue)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nRankType);
            Answer::NetPacket::writeInt64(packet, nCharId);
            Answer::NetPacket::writeInt32(packet, nValue);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EE2u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LoadZHYYHDRank(int8_t nType, int8_t nDay)
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                Answer::NetPacket::writeInt8(packet, nType);
                Answer::NetPacket::writeInt8(packet, nDay);
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4EE3u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::SaveZHYYHDRank(int8_t connid, int32_t nRankType, int32_t nCharId, int32_t nValue)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nRankType);
            Answer::NetPacket::writeInt64(packet, nCharId);
            Answer::NetPacket::writeInt32(packet, nValue);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EE4u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::onUpdateWorldBossInfo(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle world boss info update from DB
    if (pConn && inPacket)
    {
        int32_t nBossId = Answer::NetPacket::readInt32(inPacket);
        int8_t nState = Answer::NetPacket::readInt8(inPacket);
        int64_t nHp = Answer::NetPacket::readInt64(inPacket);
        CWorldBoss *boss = Answer::Singleton<CWorldBoss>::instance();
        CWorldBoss::UpdateInfo(boss, nBossId, nState, nHp);
    }
}

void DBService::onUpdateEquipBackRecord(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle equip back record update from DB
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CEquipBackRecord *record = Answer::Singleton<CEquipBackRecord>::instance();
        CEquipBackRecord::OnDBUpdate(record, Id, inPacket);
    }
}

void DBService::onUpdateEquipBackInfo(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle equip back info update from DB
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CEquipBackInfo *info = Answer::Singleton<CEquipBackInfo>::instance();
        CEquipBackInfo::OnDBUpdate(info, Id, inPacket);
    }
}

void DBService::onUpdateDoubleElevenRank(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle double eleven rank update from DB
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CFestivalDoubleEleven *fe = Answer::Singleton<CFestivalDoubleEleven>::instance();
        CFestivalDoubleEleven::OnDBUpdateRank(fe, Id, inPacket);
    }
}

void DBService::onUpdateZHYYHDRank(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle ZHYHD rank update from DB
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CZongHeYunYingHD *hd = Answer::Singleton<CZongHeYunYingHD>::instance();
        CZongHeYunYingHD::OnDBUpdateRank(hd, Id, inPacket);
    }
}

void DBService::onUpdateDropTimes(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle drop times update from DB
    if (pConn && inPacket)
    {
        int32_t nGroupId = Answer::NetPacket::readInt32(inPacket);
        int32_t nTimes = Answer::NetPacket::readInt32(inPacket);
        // Update drop times
    }
}

void DBService::onUpdateMYSJReward(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle MYSJ reward update from DB
}

void DBService::onUpdateCityWarResult(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle city war result update from DB
    if (pConn && inPacket)
    {
        int8_t nWarId = Answer::NetPacket::readInt8(inPacket);
        int64_t nFamilyId = Answer::NetPacket::readInt64(inPacket);
        CActivityManager *mgr = Answer::Singleton<CActivityManager>::instance();
        CActivityManager::SetCityWarResult(mgr, nWarId, nFamilyId);
    }
}

void DBService::onUpdateCrossTowerResutl(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle cross tower result update from DB
}

void DBService::onUpdatePeerlessWarResult(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle peerless war result update from DB
}

void DBService::onUpdateBossFirstKilled(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle boss first killed update from DB
}

void DBService::onUpdateKaiFuTime(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle kai fu time update from DB
}

void DBService::onUpdateGMBroadcast(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle GM broadcast update from DB
}

void DBService::onUpdateGMBanChat(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle GM ban chat update from DB
}

void DBService::onUpdateGMSeal(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle GM seal update from DB
}

void DBService::onLoadGMBackstage()
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4EE5u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::onBackStageKick(int8_t connid, int64_t cid)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EE6u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::ReloadTencentInfo(int8_t connid, CharId_t cid)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4EE7u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::onUpdateTencentInfo(ConnType *pConn, Answer::NetPacket *inPacket)
{
    // Handle tencent info update from DB
    if (pConn && inPacket)
    {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::onUpdateTencentInfo(v4, Id, inPacket);
    }
}

void DBService::CheckMobilePhoneGiftEffect(int8_t connid, int16_t cgindex, int32_t sid, std::string *p_passport, int8_t nType)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt16(packet, cgindex);
            Answer::NetPacket::writeInt32(packet, sid);
            Answer::NetPacket::writeUTF8(packet, p_passport);
            Answer::NetPacket::writeInt8(packet, nType);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F20u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::CheckGuardPrivilege(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int8_t type, int32_t param, int8_t index)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt16(packet, cgindex);
            Answer::NetPacket::writeInt64(packet, uid);
            Answer::NetPacket::writeInt32(packet, sid);
            Answer::NetPacket::writeInt8(packet, type);
            Answer::NetPacket::writeInt32(packet, param);
            Answer::NetPacket::writeInt8(packet, index);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F22u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::OnDBClickGame(int8_t connid, int64_t uid, int32_t sid, int32_t start_time)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, uid);
            Answer::NetPacket::writeInt32(packet, sid);
            Answer::NetPacket::writeInt32(packet, start_time);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F24u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::onLoadCityWarInfo()
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4ED2u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::onCityWarInfoLoaded(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int64_t nFamilyId = Answer::NetPacket::readInt64(inPacket);
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CActivityManager *v4 = Answer::Singleton<CActivityManager>::instance();
        CActivityManager::SetCityWarWinner(v4, Id, nFamilyId);
    }
}

void DBService::onLoadPeerlessWarInfo()
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4ED4u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::onPeerlessWarInfoLoaded(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int64_t nCharId = Answer::NetPacket::readInt64(inPacket);
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CActivityManager *v4 = Answer::Singleton<CActivityManager>::instance();
        CActivityManager::SetPeerlessWarWinner(v4, Id, nCharId);
    }
}

void DBService::onLoadCrossTowerInfo()
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4ED5u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::onLoadServerBattle()
{
    for (auto iter = this->m_mConn.begin(); iter != this->m_mConn.end(); ++iter)
    {
        ConnType *pConn = iter->second;
        if (pConn)
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
            if (packet)
            {
                int8_t Id = MutiConn<DBService>::GetId(pConn);
                Answer::NetPacket::writeInt8(packet, Id);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
                Answer::NetPacket::setProc(packet, 0x4ED6u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(pConn, packet);
            }
        }
    }
}

void DBService::onCrossTowerInfoLoaded(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int64_t nCharId = Answer::NetPacket::readInt64(inPacket);
        int32_t nBattleId = Answer::NetPacket::readInt32(inPacket);
        std::string name;
        Answer::NetPacket::readUTF8(inPacket, &name);
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        CActivityManager *mgr = Answer::Singleton<CActivityManager>::instance();
        CActivityManager::SetCrossTowerWinner(mgr, Id, nCharId, nBattleId, &name);
    }
}

void DBService::onServerBattleLoaded(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int32_t nBattleId = Answer::NetPacket::readInt32(inPacket);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::SetServerBattleState(v4, nBattleId);
    }
}

void DBService::onPlayerNetPacket(ConnType *pConn, Answer::NetPacket *inPacket)
{
    if (pConn && inPacket)
    {
        int16_t cgindex = Answer::NetPacket::readInt16(inPacket);
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        GameService *v4 = Answer::Singleton<GameService>::instance();
        GameService::OnDBNetPacket(v4, Id, cgindex, inPacket);
    }
}

void DBService::SaveEquipBackRecord(int32_t nId, int32_t nType, int32_t nTime, std::string *pName)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nId);
            Answer::NetPacket::writeInt32(packet, nType);
            Answer::NetPacket::writeInt32(packet, nTime);
            Answer::NetPacket::writeUTF8(packet, pName);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F01u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveEquipBackCount(int32_t nId, int32_t nType, int32_t nCount, std::string *pName)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nId);
            Answer::NetPacket::writeInt32(packet, nType);
            Answer::NetPacket::writeInt32(packet, nCount);
            Answer::NetPacket::writeUTF8(packet, pName);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F02u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveGuiGuDaoRenData(int32_t NpcId, int32_t Count)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, NpcId);
            Answer::NetPacket::writeInt32(packet, Count);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F03u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveBackEquipCount(int32_t Count)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, Count);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F04u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveBackEQuipRank(EquipBackRankCfg *p_stu)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            // Package EquipBackRankCfg data
            p_stu->PackageData(packet);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F05u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logAbnormal(log_abnormal *p_stu)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            // Package log_abnormal data
            Answer::NetPacket::writeInt64(packet, p_stu->nCharId);
            Answer::NetPacket::writeInt32(packet, p_stu->nType);
            Answer::NetPacket::writeUTF8(packet, &p_stu->strDesc);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F06u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::LogXinMoBag(CharId_t cid, MemChrBag item, int32_t falg, int32_t Opway)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, item.nItemId);
            Answer::NetPacket::writeInt32(packet, item.nCount);
            Answer::NetPacket::writeInt32(packet, falg);
            Answer::NetPacket::writeInt32(packet, Opway);
            Answer::NetPacket::writeInt64(packet, time(0));
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F07u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::onNewMinuteCome(int32_t minute)
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, minute);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F08u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::OnUpdatePay()
{
    ConnType *pConn = GetConn(0);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F09u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveCityWarResult(int8_t connid, int32_t nWarId, int64_t nFamilyId, int32_t nScore)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nWarId);
            Answer::NetPacket::writeInt64(packet, nFamilyId);
            Answer::NetPacket::writeInt32(packet, nScore);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F0Au);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SaveCrossTowerResult(int8_t connid, int32_t nTowerId, int64_t nCharId, int32_t nScore)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nTowerId);
            Answer::NetPacket::writeInt64(packet, nCharId);
            Answer::NetPacket::writeInt32(packet, nScore);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F0Bu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::SavePeerlessWarResult(int8_t connid, int32_t nWarId, int64_t nCharId, int32_t nScore)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nWarId);
            Answer::NetPacket::writeInt64(packet, nCharId);
            Answer::NetPacket::writeInt32(packet, nScore);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F0Cu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::logShangCheng(int8_t connid, int64_t cid, int32_t nShopId, int32_t nItemId, int32_t nCount, int32_t nPrice)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, nShopId);
            Answer::NetPacket::writeInt32(packet, nItemId);
            Answer::NetPacket::writeInt32(packet, nCount);
            Answer::NetPacket::writeInt32(packet, nPrice);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F0Du);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::OnSendMail(int8_t connid, int64_t nSenderId, int64_t nReceiverId, std::string *pTitle, std::string *pContent, std::vector<MemChrBag> *pItems)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, nSenderId);
            Answer::NetPacket::writeInt64(packet, nReceiverId);
            Answer::NetPacket::writeUTF8(packet, pTitle);
            Answer::NetPacket::writeUTF8(packet, pContent);
            Answer::NetPacket::writeInt32(packet, (int32_t)pItems->size());
            for (auto it = pItems->begin(); it != pItems->end(); ++it)
            {
                Answer::NetPacket::writeInt32(packet, it->nItemId);
                Answer::NetPacket::writeInt32(packet, it->nCount);
            }
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F0Eu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::OnDeleteMail(int8_t connid, int32_t nMailId)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nMailId);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F0Fu);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::OnUpdateMailInfo(int8_t connid, int32_t nMailId, int8_t nState)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, nMailId);
            Answer::NetPacket::writeInt8(packet, nState);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F10u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::OnSendSysMail(int8_t connid, int64_t nReceiverId, std::string *pTitle, std::string *pContent, std::vector<MemChrBag> *pItems)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, nReceiverId);
            Answer::NetPacket::writeUTF8(packet, pTitle);
            Answer::NetPacket::writeUTF8(packet, pContent);
            Answer::NetPacket::writeInt32(packet, (int32_t)pItems->size());
            for (auto it = pItems->begin(); it != pItems->end(); ++it)
            {
                Answer::NetPacket::writeInt32(packet, it->nItemId);
                Answer::NetPacket::writeInt32(packet, it->nCount);
            }
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F11u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}

void DBService::AddPlatformLog(int8_t connid, int64_t cid, int32_t nType, int32_t nValue, std::string *pDesc)
{
    ConnType *pConn = GetConn(connid);
    if (pConn)
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(pConn);
        if (packet)
        {
            Answer::NetPacket::writeInt64(packet, cid);
            Answer::NetPacket::writeInt32(packet, nType);
            Answer::NetPacket::writeInt32(packet, nValue);
            Answer::NetPacket::writeUTF8(packet, pDesc);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
            Answer::NetPacket::setProc(packet, 0x4F12u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(pConn, packet);
        }
    }
}
