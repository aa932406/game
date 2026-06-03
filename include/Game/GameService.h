#pragma once

#include <cstdint>
#include <map>
#include <list>
#include <string>
#include <vector>

#include "Answer/Mutex.h"
#include "Network/ConnStubs.h"

// Forward declarations
class Player;
class CObjPet;
class Monster;
class Trailer;
class Dungeon;
class User;
class Unit;
struct LogMoYuShiJieReward;
struct ChatValidate;
struct TencentInfo;
struct PlayerDBData;

namespace Answer {
  class Mutex;
  class MutexGuard;
  class NetPacket;
  class InetAddress;
  class MySqlDBGuard;
  class MySqlQuery;
  class DBPool;
  enum class PackType : uint8_t;
}

template<typename T> class MutiConn;
using CharId_t = int64_t;
using FamilyId_t = int64_t;
using EntityId_t = int64_t;
using PetId_t = int64_t;
using CharIdList = std::list<int64_t>;
using Int16List = std::list<int16_t>;
using IndexMap = std::map<int8_t, std::list<int16_t>>;
using PlayerList = std::list<Player*>;


class GameService {
public:
  typedef MutiConn<GameService> ConnType;

  GameService();
  ~GameService();

  GameService(const GameService&) = delete;
  GameService& operator=(const GameService&) = delete;

  static GameService* GetInstance();
  static GameService* instance() { return GetInstance(); }

  int32_t getLine() const { return m_line; }
  int32_t getId() const { return m_id; }
  
  // 静态包装方法
  static int32_t getLine(GameService* gs) { return gs ? gs->m_line : 0; }
  static Answer::NetPacket* popNetpacket(GameService* gs, int8_t connid, Answer::PackType type, uint16_t proc) { return gs ? gs->popNetpacket(connid, type, proc) : nullptr; }
  static uint32_t getWOffset() { return 0; }
  static uint32_t getWOffset(void* p) { (void)p; return 0; }
  static void sendPacket(GameService* gs, int8_t connid, Answer::NetPacket* pkt) { if (gs) gs->sendPacket(connid, pkt); }
  static void sendPacket(GameService* gs, int8_t& connid, Answer::NetPacket*& pkt) { if (gs) gs->sendPacket(connid, pkt); }
  static void sendPacket(GameService* gs, int connid, Answer::NetPacket* pkt) { if (gs) gs->sendPacket(static_cast<int8_t>(connid), pkt); }
  static void sendPacketTo(GameService* gs, int8_t connid, int16_t index, Answer::NetPacket* pkt) { if (gs) gs->sendPacketTo(connid, index, pkt); }
  static int32_t replySuccess(GameService* gs, int8_t connid, int16_t index, uint16_t proc, int64_t addon) { return gs ? gs->replySuccess(connid, index, proc, addon) : -1; }
  static int32_t replyfailure(GameService* gs, int8_t connid, int16_t index, uint16_t proc, int32_t errcode, int64_t addon) { return gs ? gs->replyfailure(connid, index, proc, errcode, addon) : -1; }
  static void worldBroadcast(GameService* gs, int8_t connid, Answer::NetPacket* pkt) { if (gs) gs->worldBroadcast(connid, pkt); }
  int32_t getWorldLevel() const { return m_WorldLevel; }
  int32_t getBattle() const { return m_Battle; }
  void setWorldLevel(int32_t level) { m_WorldLevel = level; }

  bool Connect(int8_t id, std::string *p_host, int32_t port);
  std::string GetVersion();
  void RecalAllPlayerAttr();
  void AddPlayerVipClubDropTime();
  void onNetPacket(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void startGame();
  void stopGame();
  int8_t GetAutoConnId();
  int8_t GetConnId(CharId_t cid);
  ConnType* GetConn(int8_t connid);
  static ConnType* GetConn(GameService* gs, int8_t connid) { return gs ? gs->GetConn(connid) : nullptr; }
  static ConnType* GetConn(GameService* gs) { return gs && !gs->m_mConn.empty() ? gs->m_mConn.begin()->second : nullptr; }
  Player* getUserPlayer(int8_t connid, int16_t cgindex);
  void onUpdatePay();
  void onNewMinuteCome(int32_t minute);
  void OnDaySwitch();
  void onPlayerLoaded(         PlayerDBData *const dbData,         int32_t reason,         bool isRobot);
  void onPlayerLogout(Player *player);
  void onPlayerSaved(         int8_t connid,         int64_t uid,         int32_t sid,         int32_t reason,         int32_t param,         CharId_t cid);
  void sendPacket(int8_t connid, Answer::NetPacket *packet);
  void sendPacketTo(         int8_t connid,         int16_t index,         Answer::NetPacket *inPacket);
  int32_t replySuccess(         int8_t connid,         int16_t index,         uint16_t proc,         int64_t addon);
  int32_t replyfailure(         int8_t connid,         int16_t index,         uint16_t proc,         int32_t errcode,         int64_t addon);
  void broadcastHuoDongDaTingIcon();
  void SendOpenBetaIcon();
  void broadFamilyWarIcon();
  void broadcast(         Answer::NetPacket *inPacket,         const IndexMap *const indexMap);
  void broadcast(         Answer::NetPacket *inPacket,         int8_t connid,         const Int16List *const indexList);
  void broadcast(Answer::NetPacket *inPacket);
  void broadcast(Answer::NetPacket *inPacket, const CharIdList *const cids);
  void broadcastToFamily(         Answer::NetPacket *inPacket,         FamilyId_t nFamilyId,         int32_t nLevel);
  Answer::NetPacket* popNetpacket(int8_t connid, Answer::PackType type, uint16_t proc);
  Answer::NetPacket* popNetpacket(Answer::PackType type, uint16_t proc);
  void worldBroadcast(int8_t connid, Answer::NetPacket *inPacket);
  void worldBroadcast(Answer::NetPacket *inPacket);
  void broadcastActivityState(int32_t nId, int8_t nState);
  void AddPlayer(Player *player, int32_t reason);
  void removePlayer(Player *player);
  bool SendChatValidate(         Player *Owner,         int32_t Channel,         Answer::NetPacket *inPacket);
  void addPet(CObjPet *pPet);
  void removePet(CObjPet *pPet);
  Player* getPlayer(int64_t cid, int32_t runnerId, int32_t type);
  CObjPet* getPet(int64_t petId, int32_t runnerId);
  void addMonster(Monster *monster);
  void removeMonster(Monster *monster);
  void addTrailer(Trailer *trailer);
  void removeTrailer(Trailer *trailer);
  void saveAllPlayerToDB();
  void onUserPayed(Answer::NetPacket *inPacket);
  void onAddUser(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void qqToPlayer(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onRemoveUser(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onGMReloadItem(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialNetpacket(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onGateDisconnect(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onEnterGame(         int8_t connid,         int16_t cgindex,         Answer::NetPacket *inPacket);
  void onEnterGameRobot(         int8_t connid,         int16_t cgindex,         Answer::NetPacket *inPacket);
  void onGameNetpacket(         int8_t connid,         int16_t cgindex,         Answer::NetPacket *inPacket);
  void OnDBNetPacket(         int8_t connid,         int16_t cgindex,         Answer::NetPacket *inPacket);
  void sendSocialAddPlayer(Player *player);
  void sendSocialRemovePlayer(Player *player);
  void onSocialUpdatePetRank(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialUpdateFamilyInfo(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialInitPetRank(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialInitRankMirror(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialUpdateRankMirror(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialQueryPlayerInfo(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialCreateTeamDungeon(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialSendSystemMail(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSocialSendSystemMail2(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void onSetWorldLevel(         ConnType *pConn,         Answer::NetPacket *inPacket);
  int32_t GetDropTimes(int32_t nGroupId);
  void AddDropTimes(int32_t nGroupId, int32_t nAddTimes);
  void UpdateDropTimes(int32_t nGroupId, int32_t nTimes);
  void ResetDropTimes();
  void InitDropTimes();
  void TeamDungeonEnterDungeon(         Dungeon *pDungeon,         const CharIdList *const lst);
  void OnUpdateMail(Answer::NetPacket *inPacket);
  void requestSocialData();
  void OnReceiveDaTiRank(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void OnReceiveDaTiResult(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void SendServerDiffToGlobal();
  void UpdateCityWarTitle(FamilyId_t OldFamilyId, FamilyId_t NewFamilyId);
  void UpdateCityActState(FamilyId_t FamilyId, int8_t ActState);
  void SetFamilyMemberNeedSync(FamilyId_t nFamilyId);
  void GetFamilyMemberInMap(         FamilyId_t nFamilyId,         int32_t nMapId,         PlayerList *const pList,         CharIdList *const farList,         int32_t nLevel);
  void onUpdateActivityState(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void OnSocialCityMasterBanChat(         ConnType *pConn,         Answer::NetPacket *inPacket);
  int32_t GetCityMasterBanChatTime(CharId_t nCharId);
  void KickUser(CharId_t cid, int32_t opWay);
  void KickAll(int32_t opWay);
  void onCheckTitle(CharId_t nCharId, int8_t nType, int32_t nParam);
  void onCheckTeShuTitle(CharId_t nCharId, int8_t nType, int32_t nParam);
  void onRemoveTitle(CharId_t nCharId, int8_t nType);
  void requestWorldLevel();
  void onRequestDropRecord(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void InitServerBattle();
  void SetServerBattle(int32_t Battle);
  void InitMoYuShiJieRecord();
  void SendMoYuShiJieRecord(Player *player);
  void AddMoYuShiJieRecord(const LogMoYuShiJieReward *const logReward);
  void ResetFestivalData(int32_t nVersion);
  void UpdateTencentInfo(CharId_t cid, const TencentInfo *const info);
  void FamilyBroadcast(         int8_t connid,         FamilyId_t nFamilyId,         Answer::NetPacket *inPacket);
  void onSocialInitFamilyInfo(         ConnType *pConn,         Answer::NetPacket *inPacket);
  void broadcastCityWarWinnerLogin(         CharId_t nCharId,         const std::string *const strName);
  void broadCastKiaFuHuoDongIcon();

private:

  static int32_t getUserIndex(int8_t connid, int16_t cgindex) {{
    return static_cast<int32_t>(connid) * 10000 + cgindex;
  }}

  // Members
  int32_t m_id = 0;
  int32_t m_line = 0;
  int32_t m_WorldLevel = 0;
  int32_t m_Battle = 0;
  int32_t m_ChatValidateSeq = 0;

  std::map<int8_t, ConnType*> m_mConn;
  
  Answer::Mutex m_userLock;
  std::map<int64_t, Player*> m_players;
  Answer::Mutex m_playerLock;

  std::map<int64_t, CObjPet*> m_pets;
  Answer::Mutex m_petsLock;

  std::map<int64_t, Monster*> m_monsters;
  Answer::Mutex m_monstersLock;

  std::map<int64_t, Trailer*> m_trailers;
  Answer::Mutex m_trailersLock;

  std::map<int32_t, int32_t> m_dropControl;
  Answer::Mutex m_dropControlLock;

  std::list<LogMoYuShiJieReward> m_lMYSJRecordSpecial;
  std::list<LogMoYuShiJieReward> m_lMYSJRecord;
  Answer::Mutex m_mysjRecordLock;

  std::map<int64_t, int32_t> m_mCityMasterBanChatMap;
  Answer::Mutex m_cityMasterBanChatLock;

  Answer::Mutex m_ChatValidateLock;
  std::map<int32_t, ChatValidate> m_ChatValidateMap;

  static constexpr int32_t MAX_USERS = 100 * 10000;
  User* m_users[MAX_USERS] = {{nullptr}};

  static constexpr int32_t MAX_BROADCAST = 12;
  int32_t m_onLineBroadcast[MAX_BROADCAST] = {{0}};
};