#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

#include "Answer/NetPacket.h"
#include "Database/PlayerDBData.h"

#define DB_SERVICE_TYPES_DEFINED
#ifdef DB_SERVICE_TYPES_DEFINED
typedef int64_t CharId_t;
typedef int64_t FamilyId_t;
#endif

struct MemChrBag;
typedef std::vector<MemChrBag> MemChrBagVector;

class MemEquip;
class LogTask;
class LogTask;
class LogDungeon;
class WorldBossInfo;
class FestivalRank;
struct FestivalRankVector;
class DropRecord;
class LogBoss;
class LogActivity;
class LogDailyGift;
struct LogItemChange;
class MGLog;
class LogPlayerLevel;
class Log360;
class PacketProcLog;
class Generallog;
class UpStartLog;
class DropLog;
class ChangeMapLog;
class NpcFuncLog;
struct LogMoYuShiJieReward;
class MailInfo;
class MemChrBag;

typedef std::vector<MemChrBag> MemChrBagVector;

struct LogItemChange;
class EquipBackRankCfg;
class log_abnormal;
class platform_log_base;
enum class PLATFORM_LOG_DATA_TYPE : int32_t;
struct ItemData;

class DBService {
public:
    static DBService* GetInstance();
    static DBService* instance();

    DBService();
    ~DBService();
    void Init(int32_t line);
    bool Connect(int8_t id, std::string *p_host, int32_t port);
    void onNetPacket(void *pConn, Answer::NetPacket *inPacket);
    void savePlayer(int8_t connid, int32_t reason, int32_t param, PlayerDBData *const dbData);
    void loadPlayer(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int32_t reason);
    void fengHao(int8_t connid, int8_t Type, int64_t Cid, std::string *p_name);
    void UpdateLoginTime(int8_t connid, CharId_t cid, int32_t login_time);
    void LogPlayerStay(int8_t connid, int32_t nTime, int32_t nDay);
    void insertMemEquip(int8_t connid, const MemEquip *const equip, int32_t nReason);
    void updateMemEquip(int8_t connid, const MemEquip *const equip, int32_t nReason);
    void deleteMemEquip(int8_t connid, const MemEquip *const equip, int32_t nReason);
    void insertTaskInfo(int8_t connid, const LogTask *const task);
    void updateTaskInfo(int8_t connid, const LogTask *const task);
    void insertDungeon(int8_t connid, const LogDungeon *const dungeon);
    void updateDungeon(int8_t connid, const LogDungeon *const dungeon);
    void SaveWorldBossInfo(const WorldBossInfo *const info);
    void onUpdateWorldBossInfo(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateEquipBackRecord(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateEquipBackInfo(void *pConn, Answer::NetPacket *inPacket);
    void LoadDoubleElevenRank(int8_t nType, int8_t nDay);
    void SaveDoubleElevenRank(int8_t connid, int8_t nType, int16_t nDay, int16_t nIndex, const FestivalRank *const info);
    void onUpdateDoubleElevenRank(void *pConn, Answer::NetPacket *inPacket);
    void LoadZHYYHDRank(int8_t nType, int8_t nDay);
    void SaveZHYYHDRank(int8_t connid, int8_t nType, int16_t nDay, const FestivalRankVector *const vRank);
    void onUpdateZHYYHDRank(void *pConn, Answer::NetPacket *inPacket);
    void SaveDropTimes(int32_t nGroupId, int32_t nTimes);
    void ClearDropTimes();
    void onUpdateDropTimes(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateMYSJReward(void *pConn, Answer::NetPacket *inPacket);
    void SaveDropRecord(const DropRecord *const record);
    void SaveBossFirstKilled(int32_t BossId, CharId_t Cid, std::string *p_Name, int32_t time);
    void InsertBossLog(int8_t connid, const LogBoss *const boss);
    void InsertActivityLog(int8_t connid, const LogActivity *const activity);
    void InsertDailyGiftLog(int8_t connid, const LogDailyGift *const gift);
    void insertItemChange(int8_t connid, const LogItemChange *const item);
    void insertMingGeItem(int8_t connid, const MGLog *const logStu);
    void logLevelUp(int8_t connid, const LogPlayerLevel *const logLevel);
    void SaveOnlineTime(int8_t connid, CharId_t nCharId, int32_t nDayTime, int32_t nOnlineTime);
    void log360(int8_t connid, Log360 *const LogStu);
    
    // 静态包装方法
    static void insertItemChange(DBService* db, int8_t connid, const LogItemChange* item) { if (db) db->insertItemChange(connid, item); }
    static void log360(DBService* db, int8_t connid, Log360* stu) { if (db) db->log360(connid, stu); }
    static void OnSendSysMail(DBService* db, int8_t connid, int64_t ReceiverId, int32_t SysMailId, const MemChrBagVector* items, int32_t nReason, const std::string* Param, int32_t nFamilyid) { if (db) db->OnSendSysMail(connid, ReceiverId, SysMailId, items, static_cast<ITEM_CHANGE_REASON>(nReason), Param, nFamilyid); }
    void LogProc(int8_t connid, PacketProcLog *const stu);
    void logGeneral(int8_t connid, Generallog *const LogStu);
    void LogUpEquipStar(int8_t connid, UpStartLog *const LogStu);
    void LogEquipDrop(int8_t connid, DropLog *const LogStu);
    void LogChangeMap(int8_t connid, ChangeMapLog *const LogStu);
    void LogNpcFunc(int8_t connid, NpcFuncLog *p_LogStu);
    void logMoYuShiJieReward(int8_t connid, const LogMoYuShiJieReward *const logInfo);
    void logPlayerLogin(int8_t connid, CharId_t cid, int32_t action, int32_t time, int32_t MapId);
    void logCurrency(int8_t connid, CharId_t cid, const std::string *const name, int32_t nType, int32_t opWay, int32_t nVal, int64_t nParam, int32_t nTime, int64_t LeftVal, int32_t FuLiLevel);
    void onDBPlayerLoaded(void *pConn, Answer::NetPacket *inPacket);
    void onDBPlayerSaved(void *pConn, Answer::NetPacket *inPacket);
    void onDBEquipUpdated(void *pConn, Answer::NetPacket *inPacket);
    void onDBUserPayed(void *pConn, Answer::NetPacket *inPacket);
    void logShangCheng(int8_t connid, CharId_t cid, const std::string *const name, int32_t Type, int32_t ShopId, int32_t ItemClass, int32_t ItemId, int32_t ItemCount, int32_t CostGold, int64_t LeftGold, int32_t Time);
    void OnSendMail(int8_t connid, const MailInfo *const Mail, const std::string *const Param, FamilyId_t nFamilyid);
    void OnDeleteMail(int8_t connid, int32_t nMailId);
    void OnUpdateMailInfo(void *pConn, Answer::NetPacket *inPacket);
    void OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const std::string *const Param, FamilyId_t nFamilyid);
    void OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const MemChrBag *const item, ITEM_CHANGE_REASON nReason, const std::string *const Param, FamilyId_t nFamilyid);
    void OnSendSysMail(int8_t connid, CharId_t ReceiverId, int32_t SysMailId, const MemChrBagVector *const items, ITEM_CHANGE_REASON nReason, const std::string *const Param, FamilyId_t nFamilyid);
    void onNewMinuteCome(int32_t minute);
    void OnUpdatePay();
    void SaveCityWarResult(int32_t nActId, FamilyId_t nFamilyId, int32_t nWinTime, int32_t nTime, CharId_t nLeaderCid, CharId_t First, CharId_t Second, CharId_t Third, int32_t nIndex, std::string *p_FirstFamilyName, std::string *p_SecondFamilyName, std::string *p_ThirdFamilyName);
    void SaveCrossTowerResult(int32_t nActId, CharId_t Winner, std::string *p_name, int32_t battle, int8_t connid, int32_t time);
    void onUpdateCityWarResult(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateCrossTowerResutl(void *pConn, Answer::NetPacket *inPacket);
    void SavePeerlessWarResult(int32_t nActId, CharId_t winner, std::string *p_name, int32_t nTime);
    void onUpdatePeerlessWarResult(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateBossFirstKilled(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateKaiFuTime(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateGMBroadcast(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateGMBanChat(void *pConn, Answer::NetPacket *inPacket);
    void onUpdateGMSeal(void *pConn, Answer::NetPacket *inPacket);
    void onLoadGMBackstage();
    void onBackStageKick(void *pConn, Answer::NetPacket *inPacket);
    void ReloadTencentInfo(int8_t connid, CharId_t cid);
    void onUpdateTencentInfo(void *pConn, Answer::NetPacket *inPacket);
    void CheckMobilePhoneGiftEffect(int8_t connid, int16_t cgindex, int32_t sid, std::string *p_passport, int8_t nType);
    void CheckGuardPrivilege(int8_t connid, int16_t cgindex, int64_t uid, int32_t sid, int8_t type, int32_t param, int8_t index);
    void OnDBClickGame(int8_t connid, int64_t uid, int32_t sid, int32_t start_time);
    void onLoadCityWarInfo();
    void onCityWarInfoLoaded(void *pConn, Answer::NetPacket *inPacket);
    void onLoadPeerlessWarInfo();
    void onPeerlessWarInfoLoaded(void *pConn, Answer::NetPacket *inPacket);
    void onLoadCrossTowerInfo();
    void onLoadServerBattle();
    void onCrossTowerInfoLoaded(void *pConn, Answer::NetPacket *inPacket);
    void onServerBattleLoaded(void *pConn, Answer::NetPacket *inPacket);
    void onPlayerNetPacket(void *pConn, Answer::NetPacket *inPacket);
    void SaveEquipBackRecord(int32_t nId, int8_t nType, int32_t Time, std::string *p_name);
    void SaveEquipBackCount(int32_t nId, int8_t nType, int32_t Count, std::string *p_name);
    void SaveGuiGuDaoRenData(int32_t NpcId, int32_t Count);
    void SaveBackEquipCount(int32_t Count);
    void SaveBackEQuipRank(EquipBackRankCfg *p_stu);
    void logAbnormal(log_abnormal *p_stu);
    void LogXinMoBag(CharId_t cid, MemChrBag item, int32_t falg, int32_t Opway);
    void AddPlatformLog(int8_t connid, PLATFORM_LOG_DATA_TYPE nType, platform_log_base *pData);

private:
    // TODO: member variables
};

