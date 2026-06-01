#ifndef _DBSERVICE_H_
#define _DBSERVICE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class DBService
{
public:
    DBService();
    ~DBService();

    void Init(int32_t line);
    bool Connect(int8_t id, std::string *p_host, int32_t port);
    void GetConn(int8_t connid);
    void fengHao(int8_t connid, int8_t Type, int64_t Cid, std::string *p_name);
    void UpdateLoginTime(int8_t connid, CharId_t cid, int32_t login_time);
    void LogPlayerStay(int8_t connid, int32_t nTime, int32_t nDay);
    void insertTaskInfo(int8_t connid, LogTask * task);
    void updateTaskInfo(int8_t connid, LogTask * task);
    void insertDungeon(int8_t connid, LogDungeon * dungeon);
    void updateDungeon(int8_t connid, LogDungeon * dungeon);
    void SaveWorldBossInfo(WorldBossInfo * info);
    void LoadDoubleElevenRank(int8_t nType, int8_t nDay);
    void LoadZHYYHDRank(int8_t nType, int8_t nDay);
    void SaveDropTimes(int32_t nGroupId, int32_t nTimes);
    void ClearDropTimes();
    void SaveDropRecord(DropRecord * record);
    void InsertBossLog(int8_t connid, LogBoss * boss);
    void InsertActivityLog(int8_t connid, LogActivity * activity);
    void InsertDailyGiftLog(int8_t connid, LogDailyGift * gift);
    void insertItemChange(int8_t connid, LogItemChange * item);
    void insertMingGeItem(int8_t connid, MGLog * logStu);
    void logLevelUp(int8_t connid, LogPlayerLevel * logLevel);
    void log360(int8_t connid, Log360 * LogStu);
    void LogProc(int8_t connid, PacketProcLog * stu);
    void logGeneral(int8_t connid, Generallog * LogStu);
    void LogUpEquipStar(int8_t connid, UpStartLog * LogStu);
    void LogEquipDrop(int8_t connid, DropLog * LogStu);
    void LogChangeMap(int8_t connid, ChangeMapLog * LogStu);
    void LogNpcFunc(int8_t connid, NpcFuncLog *p_LogStu);
    void OnDeleteMail(int8_t connid, int32_t nMailId);
    void onNewMinuteCome(int32_t minute);
    void OnUpdatePay();
    void onLoadGMBackstage();
    void ReloadTencentInfo(int8_t connid, CharId_t cid);
    void onLoadCityWarInfo();
    void onLoadPeerlessWarInfo();
    void onLoadCrossTowerInfo();
    void onLoadServerBattle();
    void SaveGuiGuDaoRenData(int32_t NpcId, int32_t Count);
    void SaveBackEquipCount(int32_t Count);
    void SaveBackEQuipRank(EquipBackRankCfg *p_stu);
    void logAbnormal(log_abnormal *p_stu);
    void LogXinMoBag(CharId_t cid, MemChrBag item, int32_t falg, int32_t Opway);

private:
        // TODO: 确认类型 m_mConn
};

#endif // _DBSERVICE_H_