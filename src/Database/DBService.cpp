#include "Database/DBService.h"
#include <cstdio>
#include <cstring>

// Include full type definitions for types used on stack or with method calls
#include "Game/GameService.h"
#include "Other/CEquipManager.h"
#include "Other/CEquipBack.h"
#include "Other/MailInfo.h"
#include "Other/CGMBackstate.h"
#include "Other/MGLog.h"
#include "Other/Log360.h"
#include "Other/DropLog.h"
#include "Other/NpcFuncLog.h"
#include "Other/DropRecord.h"
#include "Other/MemEquip.h"
#include "Config/CfgData.h"
#include "Config/CfgSysMail.h"
#include "Activity/CWorldBoss.h"
#include "Activity/CFestivalDoubleEleven.h"
#include "Activity/CZongHeYunYingHD.h"
#include "Activity/CActivityManager.h"
#include "Other/CKaiFuHuoDong.h"
#include "Character/CExtCharBag.h"
#include "Other/WorldBossInfo.h"
#include "Other/FestivalRank.h"
#include "Other/TencentInfo.h"
#include "Other/EquipBackRankCfg.h"
#include "Other/log_abnormal.h"
#include "Other/platform_log_base.h"
#include "Mem/MemChrBag.h"

DBService::ConnType* DBService::GetConn(int8_t id) {
    auto it = m_mConn.find(id);
    return it != m_mConn.end() ? it->second : nullptr;
}

DBService::DBService() {}
DBService::~DBService() {}

void DBService::Init(int32_t line) { (void)line; }

bool DBService::Connect(int8_t id, std::string* p_host, int32_t port) {
    // Stub - depends on MutiConn/InetAddress/TcpClient from real library
    (void)id; (void)p_host; (void)port;
    return false;
}

void DBService::onNetPacket(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    switch (Answer::NetPacket::getProc(inPacket)) {
    case 0x4EC9u: onDBPlayerLoaded(pConn, inPacket); break;
    case 0x4ECCu: onDBPlayerSaved(pConn, inPacket); break;
    case 0x4ED0u: onDBEquipUpdated(pConn, inPacket); break;
    case 0x4ED1u: onUpdateMYSJReward(pConn, inPacket); break;
    case 0x4ED3u: onCityWarInfoLoaded(pConn, inPacket); break;
    case 0x4ED7u: onPeerlessWarInfoLoaded(pConn, inPacket); break;
    case 0x4ED8u: onCrossTowerInfoLoaded(pConn, inPacket); break;
    case 0x4ED9u: onServerBattleLoaded(pConn, inPacket); break;
    case 0x4EDEu: onDBUserPayed(pConn, inPacket); break;
    case 0x4EEEu: OnUpdateMailInfo(pConn, inPacket); break;
    case 0x4EF3u: onUpdateCityWarResult(pConn, inPacket); break;
    case 0x4EF4u: onUpdateKaiFuTime(pConn, inPacket); break;
    case 0x4EF7u: onUpdateGMBroadcast(pConn, inPacket); break;
    case 0x4EF8u: onUpdateGMBanChat(pConn, inPacket); break;
    case 0x4EF9u: onUpdateGMSeal(pConn, inPacket); break;
    case 0x4EFAu: onBackStageKick(pConn, inPacket); break;
    case 0x4EFCu: onUpdateCrossTowerResutl(pConn, inPacket); break;
    case 0x4EFEu: onUpdatePeerlessWarResult(pConn, inPacket); break;
    case 0x4EFFu: onUpdateBossFirstKilled(pConn, inPacket); break;
    case 0x4F08u: onUpdateWorldBossInfo(pConn, inPacket); break;
    case 0x4F09u: onUpdateEquipBackRecord(pConn, inPacket); break;
    case 0x4F0Au: onUpdateEquipBackInfo(pConn, inPacket); break;
    case 0x4F0Du: onUpdateDoubleElevenRank(pConn, inPacket); break;
    case 0x4F10u: onUpdateZHYYHDRank(pConn, inPacket); break;
    case 0x4F13u: onUpdateDropTimes(pConn, inPacket); break;
    case 0x4F1Fu: onUpdateTencentInfo(pConn, inPacket); break;
    default: onPlayerNetPacket(pConn, inPacket); break;
    }
    Answer::NetPacket::destroy(inPacket);
}

// ---- Helper: send a packet through a connection ----
static inline bool sendPacket(DBService::ConnType* pConn, Answer::NetPacket* packet) {
    if (!packet) return false;
    Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
    Answer::NetPacket::setSize(packet, Answer::NetPacket::getWOffset(packet));
    Answer::TcpClient::sendPacket(pConn, packet);
    return true;
}

static inline Answer::NetPacket* beginPacket(DBService::ConnType* pConn) {
    if (!pConn) return nullptr;
    return Answer::TcpClient::popNetpacket(pConn);
}

// ---- Packet-sending functions follow the pattern: GetConn -> beginPacket -> write -> send ----

void DBService::savePlayer(int8_t connid, int32_t reason, int32_t param, PlayerDBData* const dbData) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, reason);
    Answer::NetPacket::writeInt32(packet, param);
    auto gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket::writeInt32(packet, gs->getId());
    dbData->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4ECAu);
    sendPacket(pConn, packet);
}

void DBService::loadPlayer(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int32_t reason) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt8(packet, connid);
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt32(packet, reason);
    auto gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket::writeInt32(packet, gs->getId());
    Answer::NetPacket::setProc(packet, 0x4EC8u);
    sendPacket(pConn, packet);
}

void DBService::fengHao(int8_t connid, int8_t Type, int64_t Cid, std::string* p_name) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt8(packet, Type);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setProc(packet, 0x4EC7u);
    sendPacket(pConn, packet);
}

void DBService::UpdateLoginTime(int8_t connid, CharId_t cid, int32_t login_time) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, login_time);
    Answer::NetPacket::setProc(packet, 0x4EF5u);
    sendPacket(pConn, packet);
}

void DBService::LogPlayerStay(int8_t connid, int32_t nTime, int32_t nDay) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt32(packet, nDay);
    Answer::NetPacket::setProc(packet, 0x4EF6u);
    sendPacket(pConn, packet);
}

void DBService::insertMemEquip(int8_t connid, const MemEquip* const equip, int32_t nReason) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nReason);
    equip->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4ECDu);
    sendPacket(pConn, packet);
}

void DBService::updateMemEquip(int8_t connid, const MemEquip* const equip, int32_t nReason) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nReason);
    equip->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4ECEu);
    sendPacket(pConn, packet);
}

void DBService::deleteMemEquip(int8_t connid, const MemEquip* const equip, int32_t nReason) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nReason);
    equip->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4ECFu);
    sendPacket(pConn, packet);
}

void DBService::insertTaskInfo(int8_t connid, const LogTask* const task) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, task->cid);
    Answer::NetPacket::writeInt32(packet, task->tid);
    Answer::NetPacket::writeInt32(packet, task->type);
    Answer::NetPacket::writeInt32(packet, task->time);
    Answer::NetPacket::setProc(packet, 0x4EDFu);
    sendPacket(pConn, packet);
}

void DBService::updateTaskInfo(int8_t connid, const LogTask* const task) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, task->cid);
    Answer::NetPacket::writeInt32(packet, task->tid);
    Answer::NetPacket::writeInt32(packet, task->type);
    Answer::NetPacket::writeInt32(packet, task->time);
    Answer::NetPacket::writeInt32(packet, task->state);
    Answer::NetPacket::setProc(packet, 0x4EE0u);
    sendPacket(pConn, packet);
}

void DBService::insertDungeon(int8_t connid, const LogDungeon* const dungeon) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, dungeon->cid);
    Answer::NetPacket::writeInt32(packet, dungeon->did);
    Answer::NetPacket::writeInt32(packet, dungeon->type);
    Answer::NetPacket::writeInt32(packet, dungeon->level);
    Answer::NetPacket::writeInt32(packet, dungeon->start_time);
    Answer::NetPacket::setProc(packet, 0x4EE1u);
    sendPacket(pConn, packet);
}

void DBService::updateDungeon(int8_t connid, const LogDungeon* const dungeon) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, dungeon->cid);
    Answer::NetPacket::writeInt32(packet, dungeon->did);
    Answer::NetPacket::writeInt32(packet, dungeon->state);
    Answer::NetPacket::writeInt32(packet, dungeon->start_time);
    Answer::NetPacket::writeInt32(packet, dungeon->finish_time);
    Answer::NetPacket::setProc(packet, 0x4EE2u);
    sendPacket(pConn, packet);
}

void DBService::SaveWorldBossInfo(const WorldBossInfo* const info) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    const_cast<WorldBossInfo*>(info)->PackageBossInfo(packet);
    Answer::NetPacket::setProc(packet, 0x4F00u);
    sendPacket(pConn, packet);
}

void DBService::onUpdateWorldBossInfo(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    WorldBossInfo info;
    info.UnPackageBossInfo(inPacket);
    auto wb = Answer::Singleton<CWorldBoss>::instance();
    wb->UpdateWorldBossInfo(&info);
}

void DBService::onUpdateEquipBackRecord(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    int32_t nTime = Answer::NetPacket::readInt32(inPacket);
    std::string sName;
    Answer::NetPacket::readUTF8(inPacket, &sName);
    auto eb = Answer::Singleton<CEquipBack>::instance();
    eb->UpdateEquipRecord(nId, nType, nTime, &sName);
}

void DBService::onUpdateEquipBackInfo(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    int32_t nCount = Answer::NetPacket::readInt32(inPacket);
    std::string sName;
    Answer::NetPacket::readUTF8(inPacket, &sName);
    auto eb = Answer::Singleton<CEquipBack>::instance();
    eb->UpdateEquipBackInfo(nId, nType, nCount, &sName);
}

void DBService::LoadDoubleElevenRank(int8_t nType, int8_t nDay) {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) return;
        Answer::NetPacket::writeInt8(packet, nType);
        Answer::NetPacket::writeInt8(packet, nDay);
        Answer::NetPacket::setProc(packet, 0x4F0Bu);
        sendPacket(pConn, packet);
    }
}

void DBService::SaveDoubleElevenRank(int8_t connid, int8_t nType, int16_t nDay, int16_t nIndex, const FestivalRank* const info) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt16(packet, nDay);
    Answer::NetPacket::writeInt16(packet, nIndex);
    Answer::NetPacket::writeInt64(packet, info->nCharId);
    Answer::NetPacket::writeUTF8(packet, &info->strName);
    Answer::NetPacket::writeInt32(packet, info->nScore);
    Answer::NetPacket::writeInt32(packet, info->nTime);
    Answer::NetPacket::setProc(packet, 0x4F0Cu);
    sendPacket(pConn, packet);
}

void DBService::onUpdateDoubleElevenRank(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int8_t nPolicy = Answer::NetPacket::readInt8(inPacket);
    int16_t type = Answer::NetPacket::readInt16(inPacket);
    int16_t count = Answer::NetPacket::readInt16(inPacket);
    FestivalRank info;
    for (int16_t i = 0; i < count; ++i) {
        int16_t idx = Answer::NetPacket::readInt16(inPacket);
        info.nCharId = Answer::NetPacket::readInt64(inPacket);
        Answer::NetPacket::readUTF8(inPacket, &info.strName);
        info.nScore = Answer::NetPacket::readInt32(inPacket);
        info.nTime = Answer::NetPacket::readInt32(inPacket);
        int8_t id = MutiConn<DBService>::GetId(pConn);
        auto fest = Answer::Singleton<CFestivalDoubleEleven>::instance();
        fest->UpdateRank(id, nPolicy, type, idx, info);
    }
}

void DBService::LoadZHYYHDRank(int8_t nType, int8_t nDay) {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) return;
        Answer::NetPacket::writeInt8(packet, nType);
        Answer::NetPacket::writeInt8(packet, nDay);
        Answer::NetPacket::setProc(packet, 0x4F0Eu);
        sendPacket(pConn, packet);
    }
}

void DBService::SaveZHYYHDRank(int8_t connid, int8_t nType, int16_t nDay, const FestivalRankVector* const vRank) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt16(packet, nDay);
    Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(vRank->size()));
    for (const auto& rank : *vRank) {
        Answer::NetPacket::writeInt16(packet, rank.nIndex);
        Answer::NetPacket::writeInt64(packet, rank.nCharId);
        Answer::NetPacket::writeUTF8(packet, &rank.strName);
        Answer::NetPacket::writeInt32(packet, rank.nScore);
        Answer::NetPacket::writeInt32(packet, rank.nTime);
    }
    Answer::NetPacket::setProc(packet, 0x4F0Fu);
    sendPacket(pConn, packet);
}

void DBService::onUpdateZHYYHDRank(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    int16_t nDay = Answer::NetPacket::readInt16(inPacket);
    int16_t nSize = Answer::NetPacket::readInt16(inPacket);
    FestivalRankVector vRank;
    vRank.resize(nSize);
    for (int16_t i = 0; i < nSize; ++i) {
        vRank[i].nIndex = Answer::NetPacket::readInt16(inPacket);
        vRank[i].nCharId = Answer::NetPacket::readInt64(inPacket);
        Answer::NetPacket::readUTF8(inPacket, &vRank[i].strName);
        vRank[i].nScore = Answer::NetPacket::readInt32(inPacket);
        vRank[i].nTime = Answer::NetPacket::readInt32(inPacket);
    }
    int8_t id = MutiConn<DBService>::GetId(pConn);
    auto zhyy = Answer::Singleton<CZongHeYunYingHD>::instance();
    zhyy->UpdateRankInfo(id, nType, nDay, vRank);
}

void DBService::SaveDropTimes(int32_t nGroupId, int32_t nTimes) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nGroupId);
    Answer::NetPacket::writeInt32(packet, nTimes);
    Answer::NetPacket::setProc(packet, 0x4F11u);
    sendPacket(pConn, packet);
}

void DBService::ClearDropTimes() {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::setProc(packet, 0x4F12u);
    sendPacket(pConn, packet);
}

void DBService::onUpdateDropTimes(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t nGroupId = Answer::NetPacket::readInt32(inPacket);
    int32_t nTimes = Answer::NetPacket::readInt32(inPacket);
    auto gs = Answer::Singleton<GameService>::instance();
    gs->UpdateDropTimes(nGroupId, nTimes);
}

void DBService::onUpdateMYSJReward(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    LogMoYuShiJieReward logInfo;
    memset(&logInfo, 0, sizeof(logInfo));
    logInfo.cid = Answer::NetPacket::readInt64(inPacket);
    std::string name;
    Answer::NetPacket::readUTF8(inPacket, &name);
    snprintf(logInfo.name, sizeof(logInfo.name), "%s", name.c_str());
    logInfo.dungeonid = Answer::NetPacket::readInt32(inPacket);
    logInfo.itemid = Answer::NetPacket::readInt32(inPacket);
    logInfo.itemclass = Answer::NetPacket::readInt8(inPacket);
    logInfo.itemcount = Answer::NetPacket::readInt32(inPacket);
    logInfo.time = Answer::NetPacket::readInt32(inPacket);
    logInfo.special = Answer::NetPacket::readInt8(inPacket);
    auto gs = Answer::Singleton<GameService>::instance();
    gs->AddMoYuShiJieRecord(&logInfo);
}

void DBService::SaveDropRecord(const DropRecord* const record) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeUTF8(packet, &record->strName);
    Answer::NetPacket::writeInt64(packet, record->nCharId);
    Answer::NetPacket::writeInt32(packet, record->nMapId);
    Answer::NetPacket::writeInt32(packet, record->nMid);
    Answer::NetPacket::writeInt32(packet, record->nRecord);
    Answer::NetPacket::writeInt32(packet, record->nTime);
    Answer::NetPacket::writeInt8(packet, record->nSpecial);
    Answer::NetPacket::setProc(packet, 0x4F14u);
    sendPacket(pConn, packet);
}

void DBService::SaveBossFirstKilled(int32_t BossId, CharId_t Cid, std::string* p_Name, int32_t time) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeUTF8(packet, p_Name);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeInt32(packet, BossId);
    Answer::NetPacket::writeInt32(packet, time);
    Answer::NetPacket::setProc(packet, 0x4F15u);
    sendPacket(pConn, packet);
}

void DBService::InsertBossLog(int8_t connid, const LogBoss* const boss) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, boss->mid);
    Answer::NetPacket::writeInt32(packet, boss->type);
    Answer::NetPacket::writeInt64(packet, boss->killer);
    Answer::NetPacket::writeInt32(packet, boss->time);
    Answer::NetPacket::writeInt32(packet, boss->mapId);
    Answer::NetPacket::writeInt32(packet, boss->MapMonsterId);
    Answer::NetPacket::writeInt64(packet, boss->EntityId);
    Answer::NetPacket::setProc(packet, 0x4EE7u);
    sendPacket(pConn, packet);
}

void DBService::InsertActivityLog(int8_t connid, const LogActivity* const activity) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, activity->cid);
    Answer::NetPacket::writeInt32(packet, activity->actid);
    Answer::NetPacket::writeInt32(packet, activity->acttype);
    Answer::NetPacket::writeInt32(packet, activity->time);
    Answer::NetPacket::writeInt64(packet, activity->param);
    Answer::NetPacket::setProc(packet, 0x4EE4u);
    sendPacket(pConn, packet);
}

void DBService::InsertDailyGiftLog(int8_t connid, const LogDailyGift* const gift) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, gift->cid);
    Answer::NetPacket::writeInt32(packet, gift->type);
    Answer::NetPacket::writeInt32(packet, gift->giftid);
    Answer::NetPacket::writeInt32(packet, gift->time);
    Answer::NetPacket::setProc(packet, 0x4EE8u);
    sendPacket(pConn, packet);
}

void DBService::insertItemChange(int8_t connid, const LogItemChange* const item) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, item->cid);
    Answer::NetPacket::writeUTF8(packet, std::string(item->name));
    Answer::NetPacket::writeInt8(packet, item->flag);
    Answer::NetPacket::writeInt32(packet, item->reason);
    Answer::NetPacket::writeInt32(packet, item->id);
    Answer::NetPacket::writeInt8(packet, item->itemClass);
    Answer::NetPacket::writeInt32(packet, item->count);
    Answer::NetPacket::writeInt32(packet, item->time);
    Answer::NetPacket::writeInt64(packet, item->srcId);
    Answer::NetPacket::writeInt32(packet, item->allCount);
    Answer::NetPacket::setProc(packet, 0x4EE5u);
    sendPacket(pConn, packet);
}

void DBService::insertMingGeItem(int8_t connid, const MGLog* const logStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    logStu->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4EE6u);
    sendPacket(pConn, packet);
}

void DBService::logLevelUp(int8_t connid, const LogPlayerLevel* const logLevel) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, logLevel->cid);
    Answer::NetPacket::writeUTF8(packet, &logLevel->name);
    Answer::NetPacket::writeInt32(packet, logLevel->level);
    Answer::NetPacket::writeInt32(packet, logLevel->time);
    Answer::NetPacket::setProc(packet, 0x4EDDu);
    sendPacket(pConn, packet);
}

void DBService::SaveOnlineTime(int8_t connid, CharId_t nCharId, int32_t nDayTime, int32_t nOnlineTime) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, nCharId);
    Answer::NetPacket::writeInt32(packet, nDayTime);
    Answer::NetPacket::writeInt32(packet, nOnlineTime);
    Answer::NetPacket::setProc(packet, 0x4F1Du);
    sendPacket(pConn, packet);
}

void DBService::log360(int8_t connid, Log360* const LogStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeUTF8(packet, &LogStu->Interface);
    Answer::NetPacket::writeInt32(packet, LogStu->UseSid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->passport);
    Answer::NetPacket::writeUTF8(packet, &LogStu->LogString);
    Answer::NetPacket::writeInt64(packet, LogStu->Cid);
    Answer::NetPacket::setProc(packet, 0x4F16u);
    sendPacket(pConn, packet);
}

void DBService::LogProc(int8_t connid, PacketProcLog* const stu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, stu->nCid);
    Answer::NetPacket::writeUTF8(packet, &stu->nName);
    Answer::NetPacket::writeInt32(packet, stu->nProcId);
    Answer::NetPacket::writeInt32(packet, stu->nTime);
    Answer::NetPacket::setProc(packet, 0x4F17u);
    sendPacket(pConn, packet);
}

void DBService::logGeneral(int8_t connid, Generallog* const LogStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, LogStu->Time);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sString);
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::setProc(packet, 0x4F18u);
    sendPacket(pConn, packet);
}

void DBService::LogUpEquipStar(int8_t connid, UpStartLog* const LogStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->nName);
    Answer::NetPacket::writeInt32(packet, LogStu->nNewStar);
    Answer::NetPacket::writeInt32(packet, LogStu->nEquipId);
    Answer::NetPacket::writeInt64(packet, LogStu->nSrcId);
    Answer::NetPacket::writeInt32(packet, LogStu->nOpway);
    Answer::NetPacket::setProc(packet, 0x4F1Au);
    sendPacket(pConn, packet);
}

void DBService::LogEquipDrop(int8_t connid, DropLog* const LogStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sName);
    Answer::NetPacket::writeInt32(packet, LogStu->nEquipId);
    Answer::NetPacket::writeInt64(packet, LogStu->nSrcId);
    Answer::NetPacket::writeInt32(packet, LogStu->nMapId);
    Answer::NetPacket::writeInt64(packet, LogStu->nKiller);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sKillerName);
    Answer::NetPacket::setProc(packet, 0x4F1Bu);
    sendPacket(pConn, packet);
}

void DBService::LogChangeMap(int8_t connid, ChangeMapLog* const LogStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &LogStu->sName);
    Answer::NetPacket::writeInt32(packet, LogStu->nCurMapId);
    Answer::NetPacket::writeInt32(packet, LogStu->nCurX);
    Answer::NetPacket::writeInt32(packet, LogStu->nCurY);
    Answer::NetPacket::writeInt32(packet, LogStu->nTarMapId);
    Answer::NetPacket::writeInt32(packet, LogStu->nTarX);
    Answer::NetPacket::writeInt32(packet, LogStu->nTarY);
    Answer::NetPacket::setProc(packet, 0x4F1Cu);
    sendPacket(pConn, packet);
}

void DBService::LogNpcFunc(int8_t connid, NpcFuncLog* p_LogStu) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, p_LogStu->nCid);
    Answer::NetPacket::writeUTF8(packet, &p_LogStu->sName);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nFuncId);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nMapId);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nX);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nY);
    Answer::NetPacket::writeInt32(packet, p_LogStu->nTime);
    Answer::NetPacket::setProc(packet, 0x4F19u);
    sendPacket(pConn, packet);
}

void DBService::logMoYuShiJieReward(int8_t connid, const LogMoYuShiJieReward* const logInfo) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, logInfo->cid);
    std::string val(logInfo->name);
    Answer::NetPacket::writeUTF8(packet, &val);
    Answer::NetPacket::writeInt32(packet, logInfo->dungeonid);
    Answer::NetPacket::writeInt32(packet, logInfo->itemid);
    Answer::NetPacket::writeInt8(packet, logInfo->itemclass);
    Answer::NetPacket::writeInt32(packet, logInfo->itemcount);
    Answer::NetPacket::writeInt32(packet, logInfo->time);
    Answer::NetPacket::writeInt8(packet, logInfo->special);
    Answer::NetPacket::setProc(packet, 0x4EDAu);
    sendPacket(pConn, packet);
}

void DBService::logPlayerLogin(int8_t connid, CharId_t cid, int32_t action, int32_t time, int32_t MapId) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, action);
    Answer::NetPacket::writeInt32(packet, time);
    Answer::NetPacket::setProc(packet, 0x4EDBu);
    sendPacket(pConn, packet);
}

void DBService::logCurrency(int8_t connid, CharId_t cid, const std::string* const name, int32_t nType, int32_t opWay, int32_t nVal, int64_t nParam, int32_t nTime, int64_t LeftVal, int32_t FuLiLevel) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeUTF8(packet, name);
    Answer::NetPacket::writeInt32(packet, nType);
    Answer::NetPacket::writeInt32(packet, opWay);
    Answer::NetPacket::writeInt32(packet, nVal);
    Answer::NetPacket::writeInt64(packet, nParam);
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt64(packet, LeftVal);
    Answer::NetPacket::writeInt32(packet, FuLiLevel);
    Answer::NetPacket::setProc(packet, 0x4EEBu);
    sendPacket(pConn, packet);
}

void DBService::onDBPlayerLoaded(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t reason = Answer::NetPacket::readInt32(inPacket);
    PlayerDBData dbData;
    PlayerDBData::UnPackageData(&dbData, inPacket, 0);
    auto gs = Answer::Singleton<GameService>::instance();
    gs->onPlayerLoaded(&dbData, reason, 0);
}

void DBService::onDBPlayerSaved(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    CharId_t cid = Answer::NetPacket::readInt64(inPacket);
    std::string name;
    Answer::NetPacket::readUTF8(inPacket, &name);
    int32_t ticks = Answer::NetPacket::readInt32(inPacket);
    int64_t uid = Answer::NetPacket::readInt64(inPacket);
    int32_t sid = Answer::NetPacket::readInt32(inPacket);
    int32_t reason = Answer::NetPacket::readInt32(inPacket);
    int32_t param = Answer::NetPacket::readInt32(inPacket);
    Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_DEBUG, "use %d ticks to save player cid = %lld, name = %s\n", ticks, cid, name.c_str());
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto gs = Answer::Singleton<GameService>::instance();
    gs->onPlayerSaved(Id, uid, sid, reason, param, cid);
}

void DBService::onDBEquipUpdated(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    MemEquipVector vEquip;
    MemEquip equip;
    int32_t count = Answer::NetPacket::readInt32(inPacket);
    vEquip.reserve(count);
    for (int32_t i = 0; i < count; ++i) {
        equip.CleanUp();
        equip.UnPackageData(inPacket);
        vEquip.push_back(equip);
    }
    auto eqmgr = Answer::Singleton<CEquipManager>::instance();
    eqmgr->OnEquipUpdated(&vEquip);
}

void DBService::onDBUserPayed(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    auto gs = Answer::Singleton<GameService>::instance();
    gs->onUserPayed(inPacket);
}

void DBService::logShangCheng(int8_t connid, CharId_t cid, const std::string* const name, int32_t Type, int32_t ShopId, int32_t ItemClass, int32_t ItemId, int32_t ItemCount, int32_t CostGold, int64_t LeftGold, int32_t Time) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
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
    Answer::NetPacket::setProc(packet, 0x4EE9u);
    sendPacket(pConn, packet);
}

void DBService::OnSendMail(int8_t connid, const MailInfo* const Mail, const std::string* const Param, FamilyId_t nFamilyid) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
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
    if (Mail->Extract == 1) {
        int8_t Count = 0;
        int32_t Offset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::writeInt8(packet, 0);
        for (int8_t i = 0; i <= 5; ++i) {
            if (Mail->Item[i].itemId > 0 && Mail->Item[i].itemCount > 0) {
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
    Answer::NetPacket::setProc(packet, 0x4EECu);
    sendPacket(pConn, packet);
}

void DBService::OnDeleteMail(int8_t connid, int32_t nMailId) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nMailId);
    Answer::NetPacket::setProc(packet, 0x4EEDu);
    sendPacket(pConn, packet);
}

void DBService::OnUpdateMailInfo(ConnType* pConn, Answer::NetPacket* inPacket) {
    (void)pConn;
    auto gs = Answer::Singleton<GameService>::instance();
    gs->OnUpdateMail(inPacket);
}

void DBService::OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const std::string* const Param, FamilyId_t nFamilyid) {
    MailInfo Mail;
    memset(&Mail, 0, sizeof(Mail));
    auto cfg = Answer::Singleton<CfgData>::instance();
    CfgSysMail* pSysMail = cfg->GetSysMail(SysMailId);
    if (!pSysMail) return;
    if (pSysMail->sender_name.empty()) return;
    if (StringUtility::utf8Strlen(pSysMail->sender_name.c_str()) > 30) return;
    if (pSysMail->title.empty()) return;
    if (StringUtility::utf8Strlen(pSysMail->title.c_str()) > 30) return;
    if (pSysMail->content.empty()) return;
    if (StringUtility::utf8Strlen(pSysMail->content.c_str()) > 500) return;
    Mail.SysMailId = SysMailId;
    Mail.ReceiveId = ReceiverId;
    Mail.SendTime = Answer::DayTime::now();
    Mail.Param = *Param;
    Mail.nReason = 2;
    int32_t FuJianCount = static_cast<int32_t>(pSysMail->item.size());
    if (FuJianCount > 0 && FuJianCount <= 5) {
        Mail.Extract = 1;
        for (int32_t i = 0; i < FuJianCount; ++i) {
            Mail.Item[i] = pSysMail->item[i];
        }
    }
    this->OnSendMail(connid, &Mail, Param, nFamilyid);
}

void DBService::OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const MemChrBag* const item, ITEM_CHANGE_REASON nReason, const std::string* const Param, FamilyId_t nFamilyid) {
    MemChrBagVector items;
    if (item->itemId > 0 && item->itemCount > 0) {
        items.push_back(*item);
    }
    this->OnSendSysMail(connid, ReceiverId, SysMailId, &items, nReason, Param, nFamilyid);
}

void DBService::OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const MemChrBagVector* const items, ITEM_CHANGE_REASON nReason, const std::string* const Param, FamilyId_t nFamilyid) {
    MailInfo Mail;
    memset(&Mail, 0, sizeof(Mail));
    auto cfg = Answer::Singleton<CfgData>::instance();
    CfgSysMail* pSysMail = cfg->GetSysMail(SysMailId);
    if (!pSysMail) return;
    if (pSysMail->sender_name.empty()) return;
    if (StringUtility::utf8Strlen(pSysMail->sender_name.c_str()) > 30) return;
    if (pSysMail->title.empty()) return;
    if (StringUtility::utf8Strlen(pSysMail->title.c_str()) > 30) return;
    if (pSysMail->content.empty()) return;
    if (StringUtility::utf8Strlen(pSysMail->content.c_str()) > 500) return;
    Mail.SysMailId = SysMailId;
    Mail.ReceiveId = ReceiverId;
    Mail.SendTime = Answer::DayTime::now();
    Mail.Param = *Param;
    Mail.nReason = static_cast<int32_t>(nReason);
    int32_t FuJianCount = static_cast<int32_t>(items->size());
    if (FuJianCount > 0 && FuJianCount <= 5) {
        Mail.Extract = 1;
        for (int32_t i = 0; i < FuJianCount; ++i) {
            Mail.Item[i] = (*items)[i];
        }
    }
    this->OnSendMail(connid, &Mail, Param, nFamilyid);
}

void DBService::onNewMinuteCome(int32_t minute) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, minute);
    Answer::NetPacket::setProc(packet, 0x4EEFu);
    sendPacket(pConn, packet);
}

void DBService::OnUpdatePay() {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt8(packet, 0);
    Answer::NetPacket::setProc(packet, 0x4EF0u);
    sendPacket(pConn, packet);
}

void DBService::SaveCityWarResult(int32_t nActId, FamilyId_t nFamilyId, int32_t nWinTime, int32_t nTime, CharId_t nLeaderCid, CharId_t First, CharId_t Second, CharId_t Third, int32_t nIndex, std::string* p_FirstFamilyName, std::string* p_SecondFamilyName, std::string* p_ThirdFamilyName) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
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
    Answer::NetPacket::setProc(packet, 0x4EF1u);
    sendPacket(pConn, packet);
}

void DBService::SaveCrossTowerResult(int32_t nActId, CharId_t Winner, std::string* p_name, int32_t battle, int8_t connid, int32_t time) {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) return;
        if (MutiConn<DBService>::GetId(pConn) == connid) {
            Answer::NetPacket::writeInt32(packet, nActId);
            Answer::NetPacket::writeInt64(packet, Winner);
            Answer::NetPacket::writeUTF8(packet, p_name);
            Answer::NetPacket::writeInt32(packet, battle);
        } else {
            Answer::NetPacket::writeInt32(packet, nActId);
            Answer::NetPacket::writeInt64(packet, 0);
            Answer::NetPacket::writeUTF8(packet, "");
            Answer::NetPacket::writeInt32(packet, 0);
        }
        Answer::NetPacket::writeInt32(packet, time);
        Answer::NetPacket::setProc(packet, 0x4EF2u);
        sendPacket(pConn, packet);
    }
}

void DBService::onUpdateCityWarResult(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t nActId = Answer::NetPacket::readInt32(inPacket);
    FamilyId_t nFamilyId = Answer::NetPacket::readInt64(inPacket);
    int32_t nWinTime = Answer::NetPacket::readInt32(inPacket);
    int32_t nTime = Answer::NetPacket::readInt32(inPacket);
    CharId_t nLeaderCid = Answer::NetPacket::readInt64(inPacket);
    CharId_t First = Answer::NetPacket::readInt64(inPacket);
    CharId_t Second = Answer::NetPacket::readInt64(inPacket);
    CharId_t Third = Answer::NetPacket::readInt64(inPacket);
    int32_t nIndex = Answer::NetPacket::readInt32(inPacket);
    std::string FirstFamilyName, SecondFamilyName, ThirdFamilyName;
    Answer::NetPacket::readUTF8(inPacket, &FirstFamilyName);
    Answer::NetPacket::readUTF8(inPacket, &SecondFamilyName);
    Answer::NetPacket::readUTF8(inPacket, &ThirdFamilyName);
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto actMgr = Answer::Singleton<CActivityManager>::instance();
    actMgr->OnCityWarResult(Id, nActId, nFamilyId, nWinTime, nTime, nLeaderCid, First, Second, Third, nIndex, FirstFamilyName, SecondFamilyName, ThirdFamilyName);
}

void DBService::onUpdateCrossTowerResutl(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t nActId = Answer::NetPacket::readInt32(inPacket);
    CharId_t Winner = Answer::NetPacket::readInt64(inPacket);
    int32_t Battle = Answer::NetPacket::readInt32(inPacket);
    std::string Name;
    Answer::NetPacket::readUTF8(inPacket, &Name);
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto actMgr = Answer::Singleton<CActivityManager>::instance();
    actMgr->OnCrossTower(Id, nActId, Winner, Name, Battle);
}

void DBService::SavePeerlessWarResult(int32_t nActId, CharId_t winner, std::string* p_name, int32_t nTime) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nActId);
    Answer::NetPacket::writeInt32(packet, nTime);
    Answer::NetPacket::writeInt64(packet, winner);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setProc(packet, 0x4EFDu);
    sendPacket(pConn, packet);
}

void DBService::onUpdatePeerlessWarResult(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t nActId = Answer::NetPacket::readInt32(inPacket);
    CharId_t winner = Answer::NetPacket::readInt64(inPacket);
    std::string name;
    Answer::NetPacket::readUTF8(inPacket, &name);
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto actMgr = Answer::Singleton<CActivityManager>::instance();
    actMgr->OnPeerlessWarResult(Id, nActId, winner, name);
}

void DBService::onUpdateBossFirstKilled(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    std::string strName;
    Answer::NetPacket::readUTF8(inPacket, &strName);
    CharId_t nCharId = Answer::NetPacket::readInt64(inPacket);
    int32_t BossId = Answer::NetPacket::readInt32(inPacket);
    int32_t time = Answer::NetPacket::readInt32(inPacket);
    auto kfhd = Answer::Singleton<CKaiFuHuoDong>::instance();
    kfhd->OnKilledMonster(nullptr, BossId);  // TODO: 正确调用
}

void DBService::onUpdateKaiFuTime(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int32_t kaiFuTime = Answer::NetPacket::readInt32(inPacket);
    auto cfg = Answer::Singleton<CfgData>::instance();
    cfg->updateServerStartTime(kaiFuTime);
}

void DBService::onUpdateGMBroadcast(ConnType* pConn, Answer::NetPacket* inPacket) {
    (void)pConn;
    auto gm = Answer::Singleton<CGMBackstate>::instance();
    gm->onUpdateGMBroadcast(inPacket);
}

void DBService::onUpdateGMBanChat(ConnType* pConn, Answer::NetPacket* inPacket) {
    auto gm = Answer::Singleton<CGMBackstate>::instance();
    gm->onUpdateGMBanChat(inPacket);
    auto gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 1) {
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        auto packet = GameService::popNetpacket(gs, Id, Answer::PackType::PACK_DISPATCH, 0x4E4Eu);
        if (packet) {
            uint32_t Size = Answer::NetPacket::getSize(inPacket);
            char* Buffer = Answer::NetPacket::getBuffer(inPacket);
            Answer::NetPacket::write(packet, Buffer, Size);
            Answer::NetPacket::setSize(packet, Answer::NetPacket::getWOffset(packet));
            gs->sendPacket(MutiConn<DBService>::GetId(pConn), packet);
        }
    }
}

void DBService::onUpdateGMSeal(ConnType* pConn, Answer::NetPacket* inPacket) {
    (void)pConn;
    auto gm = Answer::Singleton<CGMBackstate>::instance();
    gm->onUpdateGMSeal(inPacket);
}

void DBService::onLoadGMBackstage() {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) continue;
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        Answer::NetPacket::writeInt8(packet, Id);
        Answer::NetPacket::setProc(packet, 0x4EFBu);
        sendPacket(pConn, packet);
    }
}

void DBService::onBackStageKick(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    CharId_t nCharId = Answer::NetPacket::readInt64(inPacket);
    auto gs = Answer::Singleton<GameService>::instance();
    if (nCharId <= 0)
        gs->KickAll(1);
    else
        gs->KickUser(nCharId, 1);
}

void DBService::ReloadTencentInfo(int8_t connid, CharId_t cid) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::setProc(packet, 0x4F1Eu);
    sendPacket(pConn, packet);
}

void DBService::onUpdateTencentInfo(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    CharId_t cid = Answer::NetPacket::readInt64(inPacket);
    TencentInfo info;
    info.UnPacketInfo(inPacket);
    auto gs = Answer::Singleton<GameService>::instance();
    gs->UpdateTencentInfo(cid, &info);
}

void DBService::CheckMobilePhoneGiftEffect(int8_t connid, int16_t cgindex, int32_t sid, std::string* p_passport, int8_t nType) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeUTF8(packet, p_passport);
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::setProc(packet, 0x4F20u);
    sendPacket(pConn, packet);
}

void DBService::CheckGuardPrivilege(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int8_t type, int32_t param, int8_t index) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt16(packet, cgindex);
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt32(packet, param);
    Answer::NetPacket::writeInt8(packet, index);
    Answer::NetPacket::setProc(packet, 0x4F22u);
    sendPacket(pConn, packet);
}

void DBService::OnDBClickGame(int8_t connid, int64_t uid, int32_t sid, int32_t start_time) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt32(packet, start_time);
    Answer::NetPacket::setProc(packet, 0x4F24u);
    sendPacket(pConn, packet);
}

void DBService::onLoadCityWarInfo() {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) continue;
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        Answer::NetPacket::writeInt8(packet, Id);
        Answer::NetPacket::setProc(packet, 0x4ED2u);
        sendPacket(pConn, packet);
    }
}

void DBService::onCityWarInfoLoaded(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int64_t nFamilyId = Answer::NetPacket::readInt64(inPacket);
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto actMgr = Answer::Singleton<CActivityManager>::instance();
    actMgr->SetCityWarWinner(Id, nFamilyId);
}

void DBService::onLoadPeerlessWarInfo() {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) continue;
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        Answer::NetPacket::writeInt8(packet, Id);
        Answer::NetPacket::setProc(packet, 0x4ED4u);
        sendPacket(pConn, packet);
    }
}

void DBService::onPeerlessWarInfoLoaded(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int64_t nCharId = Answer::NetPacket::readInt64(inPacket);
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto actMgr = Answer::Singleton<CActivityManager>::instance();
    actMgr->SetPeerlessWarWinner(Id, nCharId);
}

void DBService::onLoadCrossTowerInfo() {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) continue;
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        Answer::NetPacket::writeInt8(packet, Id);
        Answer::NetPacket::setProc(packet, 0x4ED5u);
        sendPacket(pConn, packet);
    }
}

void DBService::onLoadServerBattle() {
    for (auto& [key, pConn] : m_mConn) {
        if (!pConn) continue;
        auto packet = beginPacket(pConn);
        if (!packet) continue;
        int8_t Id = MutiConn<DBService>::GetId(pConn);
        Answer::NetPacket::writeInt8(packet, Id);
        Answer::NetPacket::setProc(packet, 0x4ED6u);
        sendPacket(pConn, packet);
    }
}

void DBService::onCrossTowerInfoLoaded(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    CharId_t nCharId = Answer::NetPacket::readInt64(inPacket);
    int32_t nBattle = Answer::NetPacket::readInt32(inPacket);
    std::string sName;
    Answer::NetPacket::readUTF8(inPacket, &sName);
    if (nCharId > 0) {
        auto actMgr = Answer::Singleton<CActivityManager>::instance();
        actMgr->SetCrossTowerWinner(nCharId, nBattle, sName);
    }
}

void DBService::onServerBattleLoaded(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int Battle = Answer::NetPacket::readInt32(inPacket);
    if (Battle > 0) {
        auto gs = Answer::Singleton<GameService>::instance();
        gs->SetServerBattle(Battle);
    }
}

void DBService::onPlayerNetPacket(ConnType* pConn, Answer::NetPacket* inPacket) {
    if (!pConn || !inPacket) return;
    int16_t Int16 = Answer::NetPacket::readInt16(inPacket);
    int8_t Id = MutiConn<DBService>::GetId(pConn);
    auto gs = Answer::Singleton<GameService>::instance();
    gs->OnDBNetPacket(Id, Int16, inPacket);
}

void DBService::SaveEquipBackRecord(int32_t nId, int8_t nType, int32_t Time, std::string* p_name) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nId);
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt32(packet, Time);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setProc(packet, 0x4F01u);
    sendPacket(pConn, packet);
}

void DBService::SaveEquipBackCount(int32_t nId, int8_t nType, int32_t Count, std::string* p_name) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, nId);
    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::writeUTF8(packet, p_name);
    Answer::NetPacket::setProc(packet, 0x4F02u);
    sendPacket(pConn, packet);
}

void DBService::SaveGuiGuDaoRenData(int32_t NpcId, int32_t Count) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, NpcId);
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::setProc(packet, 0x4F03u);
    sendPacket(pConn, packet);
}

void DBService::SaveBackEquipCount(int32_t Count) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt32(packet, Count);
    Answer::NetPacket::setProc(packet, 0x4F04u);
    sendPacket(pConn, packet);
}

void DBService::SaveBackEQuipRank(EquipBackRankCfg* p_stu) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    p_stu->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4F05u);
    sendPacket(pConn, packet);
}

void DBService::logAbnormal(log_abnormal* p_stu) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    p_stu->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4F06u);
    sendPacket(pConn, packet);
}

void DBService::LogXinMoBag(CharId_t cid, MemChrBag item, int32_t falg, int32_t Opway) {
    auto pConn = GetConn(0);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, item.itemId);
    Answer::NetPacket::writeInt8(packet, item.itemClass);
    Answer::NetPacket::writeInt32(packet, item.itemCount);
    Answer::NetPacket::writeInt64(packet, item.srcId);
    Answer::NetPacket::writeInt32(packet, falg);
    Answer::NetPacket::writeInt32(packet, Opway);
    Answer::NetPacket::writeInt32(packet, Answer::DayTime::now());
    Answer::NetPacket::setProc(packet, 0x4F07u);
    sendPacket(pConn, packet);
}

void DBService::AddPlatformLog(int8_t connid, PLATFORM_LOG_DATA_TYPE nType, platform_log_base* pData) {
    auto pConn = GetConn(connid);
    auto packet = beginPacket(pConn);
    if (!packet) return;
    Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(nType));
    pData->PackageData(packet);
    Answer::NetPacket::setProc(packet, 0x4F23u);
    sendPacket(pConn, packet);
}
