#include "GameService.h"

#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/CObjPet.h"
#include "Game/Monster.h"
#include "Game/Trailer.h"
#include "Game/Dungeon.h"
#include "Game/Unit.h"
#include "Game/MapManager.h"
#include "Game/CPoolManager.h"
#include "Game/CTimer.h"
#include "Game/CWorldBoss.h"#include "Other/User.h"
#include "Other/CVip.h"
#include "Other/CGMBackstate.h"
#include "Other/PlayerRobot.h"
#include "Other/CPetManager.h"
#include "Other/FamilyManager.h"
#include "Character/CExtCharTitle.h"
#include "Other/CRankMirror.h"
#include "Other/TileManager.h"
#include "Other/CFestivalDoubleEleven.h"
#include "Other/CZongHeYunYingHD.h"
#include "Other/CKiaFuRecharge.h"
#include "Other/CFestivalActivity.h"
#include "Other/COpenBeta.h"
#include "Other/DayTime.h"
#include "Other/Logger.h"
#include "Activity/CActivityManager.h"
#include "Activity/CFestivalDoubleEleven.h"
#include "Activity/CZongHeYunYingHD.h"
#include "Activity/CKiaFuRecharge.h"
#include "Activity/CFestivalActivity.h"
#include "Activity/COpenBeta.h"
#include "Activity/CWorldBoss.h"
#include "Answer/NetPacket.h"
#include "Answer/Singleton.h"
#include "Answer/Logger.h"
#include "Answer/DayTime.h"
#include "Answer/Inifile.h"
#include "Common/Random.h"
#include "Database/DBService.h"

#include "Network/ConnStubs.h"
#include "Answer/MD5.h"
#include <cstring>
#include <cstdio>
#include <algorithm>

GameService::GameService() {
  /* vptr = vtable_GameService; */
  m_id = 0;
  m_line = 0;
  /* m_mConn.map(); - container init */
  m_WorldLevel = 0;
  /* m_userLock.Mutex(); - mutex init */
  /* m_players.map(); - container init */
  /* m_playerLock.Mutex(); - mutex init */
  /* m_pets.map(); - container init */
  /* m_petsLock.Mutex(); - mutex init */
  /* m_monsters.map(); - container init */
  /* m_monstersLock.Mutex(); - mutex init */
  /* m_trailers.map(); - container init */
  /* m_trailersLock.Mutex(); - mutex init */
  /* m_dropControl.map(); - container init */
  /* m_dropControlLock.Mutex(); - mutex init */
  /* m_lMYSJRecordSpecial.list(); - container init */
  /* m_lMYSJRecord.list(); - container init */
  /* m_mysjRecordLock.Mutex(); - mutex init */
  /* m_mCityMasterBanChatMap.map(); - container init */
  /* m_cityMasterBanChatLock.Mutex(); - mutex init */
  /* m_ChatValidateLock.Mutex(); - mutex init */
  /* m_ChatValidateMap.map(); - container init */
  memset(m_users, 0, sizeof(m_users));
  memset(m_onLineBroadcast, 0, sizeof(m_onLineBroadcast));
  m_WorldLevel = 0;
  m_ChatValidateSeq = 0;
  m_Battle = 0;
}

GameService::~GameService() {
  /* vptr = vtable_GameService; */
  /* m_ChatValidateMap.~map(); - container destroy */
  /* m_ChatValidateLock.~Mutex(); - mutex destroy */
  /* m_cityMasterBanChatLock.~Mutex(); - mutex destroy */
  /* m_mCityMasterBanChatMap.~map(); - container destroy */
  /* m_mysjRecordLock.~Mutex(); - mutex destroy */
  /* m_lMYSJRecord.~list(); - container destroy */
  /* m_lMYSJRecordSpecial.~list(); - container destroy */
  /* m_dropControlLock.~Mutex(); - mutex destroy */
  /* m_dropControl.~map(); - container destroy */
  /* m_trailersLock.~Mutex(); - mutex destroy */
  /* m_trailers.~map(); - container destroy */
  /* m_monstersLock.~Mutex(); - mutex destroy */
  /* m_monsters.~map(); - container destroy */
  /* m_petsLock.~Mutex(); - mutex destroy */
  /* m_pets.~map(); - container destroy */
  /* m_playerLock.~Mutex(); - mutex destroy */
  /* m_players.~map(); - container destroy */
  /* m_userLock.~Mutex(); - mutex destroy */
  /* m_mConn.~map(); - container destroy */
}

bool GameService::Connect(int8_t id, std::string *p_host, int32_t port)
{
  ConnType *v5;   /* var */ConnType **v7; 

  std::string name;
  std::string val;
  std::string v17;
  std::string v18;
  ConnType *pConn;
  Answer::NetPacket *packet;

  if ( id < 0 || id > 99 )
    return 0;
      v5 = new ConnType(0x2000, 0x2000, 0x7F9u, 0, 0x7D0u, 0, name, id, this);
  pConn = v5;
      if ( !pConn )
    return 0;
  Answer::InetAddress serverAddr(p_host, port);
  if (!pConn->connect(&serverAddr))
    return 0;
  pConn->start();
  this->m_mConn[id] = pConn;
  packet = pConn->popNetpacket();
  if ( !packet )
    return 0;
  packet->writeInt32(this->m_line);
  val = GetVersion();
  Answer::NetPacket::writeUTF8(packet, &val);
    v17 = GetVersion();
  Answer::NetPacket::writeUTF8(packet, &v17);
    v18 = "";
  Answer::NetPacket::writeUTF8(packet, &v18);
  uint32_t wOffset = 0;
    wOffset = packet->getWOffset();
  packet->setSize(wOffset);
  Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
  packet->setProc(0x4E22u);
  pConn->sendPacket(packet);
  return 1;
}


std::string GameService::GetVersion()
{
  return "1.0.0";
}


void GameService::RecalAllPlayerAttr()
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    pPlayer = iter->second;
    if ( pPlayer )
    {
      pPlayer->RecalcAttr();
      break;
    }
  }
  /* MutexGuard destroyed */
}


void GameService::AddPlayerVipClubDropTime()
{
  CVip *PlayerVip; 
  Answer::DBPool *v2; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    pPlayer = iter->second;
    if ( pPlayer )
    {
      PlayerVip = pPlayer->GetPlayerVip();
      PlayerVip->AddClubDropTime();
    }
  }
  v2 = Answer::Singleton<Answer::DBPool>::instance();
  Answer::MySqlDBGuard db(v2);
  Answer::MySqlDBGuard::excute(&db, "update mem_chr_vip set drop_time = 1 where club > 0 and drop_time <= 0");
  /* db auto-destructed */
  /* MutexGuard destroyed */
}


void GameService::onNetPacket( ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t Id; 
  CZongHeYunYingHD *v4; 
  if ( pConn && inPacket )
  {
    uint16_t proc = inPacket->getProc();
    int8_t connid = pConn->GetId();
    if ( proc <= 0x4E20u || proc > 0x61AAu )
    {
      int32_t cgindex = inPacket->readInt32();
      if ( cgindex > 0 && cgindex <= 9999 )
      {
        if ( proc == 6 )
        {
          onEnterGame(connid, cgindex, inPacket);
        }
        else if ( proc == 7 )
        {
          onEnterGameRobot(connid, cgindex, inPacket);
        }
        else
        {
          onGameNetpacket(connid, cgindex, inPacket);
        }
      }
    }
    else if ( proc <= 0x4E54u || proc > 0x4E77u )
    {
      switch ( proc )
      {
        case 0x4E26u:
          onAddUser(pConn, inPacket);
          break;
        case 0x4E27u:
          onRemoveUser(pConn, inPacket);
          break;
        case 0x4E29u:
          onGMReloadItem(pConn, inPacket);
          break;
        case 0x4E2Du:
          onGateDisconnect(pConn, inPacket);
          break;
        case 0x4E7Eu:
          onSocialUpdatePetRank(pConn, inPacket);
          break;
        case 0x4E7Fu:
          onSocialUpdateFamilyInfo(pConn, inPacket);
          break;
        case 0x4E80u:
          onSocialUpdateRankMirror(pConn, inPacket);
          break;
        case 0x4E81u:
          onSetWorldLevel(pConn, inPacket);
          break;
        case 0x4E82u:
          OnReceiveDaTiResult(pConn, inPacket);
          break;
        case 0x4E83u:
          OnSocialCityMasterBanChat(pConn, inPacket);
          break;
        case 0x4E86u:
          onSocialInitPetRank(pConn, inPacket);
          break;
        case 0x4E87u:
          onSocialCreateTeamDungeon(pConn, inPacket);
          break;
        case 0x4E88u:
          OnReceiveDaTiRank(pConn, inPacket);
          break;
        case 0x4E89u:
          onSocialSendSystemMail(pConn, inPacket);
          break;
        case 0x4E8Au:
          onSocialSendSystemMail2(pConn, inPacket);
          break;
        case 0x4E8Bu:
          onSocialInitRankMirror(pConn, inPacket);
          break;
        case 0x4E8Cu:
          onSocialQueryPlayerInfo(pConn, inPacket);
          break;
        case 0x4E8Du:
          onRequestDropRecord(pConn, inPacket);
          break;
        case 0x4E8Eu:
          onSocialInitFamilyInfo(pConn, inPacket);
          break;
        case 0x4EC1u:
          Id = pConn->GetId();
          v4 = Answer::Singleton<CZongHeYunYingHD>::instance();
          v4->UpdatePlayerInfo(Id, inPacket);
          break;
        case 0x4EC4u:
          onUpdateActivityState(pConn, inPacket);
          break;
        case 0x4EEAu:
          qqToPlayer(pConn, inPacket);
          break;
        default:
          break;
      }
    }
    else
    {
      onSocialNetpacket(pConn, inPacket);
    }
    inPacket->destroy();
  }
}


void GameService::startGame()
{
  TileManager *v1; 
  MapManager *v2; 
  MapManager *v3; 

  CActivityManager *v5; 

  CGMBackstate *v7; 

  CFestivalDoubleEleven *v9; 

  CZongHeYunYingHD *v11; 

  CKiaFuRecharge *v13; 
  CFestivalActivity *v14; 
  GameService *v15; 
  CfgData *v16; 


  GameService *v19; 
  Answer::NetPacket *packet;

  /* Player::initNetPacketHandlers commented - needs static wrapper */
  InitServerBattle();
  v1 = Answer::Singleton<TileManager>::instance();
  v1->Init();
  v2 = Answer::Singleton<MapManager>::instance();
  /* v2->Init() not available on MapManager */
  v3 = Answer::Singleton<MapManager>::instance();
  v3->StartAll();
  m_line = this->m_line;
  v5 = Answer::Singleton<CActivityManager>::instance();
  v5->Init(m_line);
  v7 = Answer::Singleton<CGMBackstate>::instance();
  v7->Init(this->m_line);
  v9 = Answer::Singleton<CFestivalDoubleEleven>::instance();
  v9->Init(this->m_line);
  v11 = Answer::Singleton<CZongHeYunYingHD>::instance();
  v11->Init(this->m_line);
  v13 = Answer::Singleton<CKiaFuRecharge>::instance();
  v13->Init(this->m_line);
  v14 = Answer::Singleton<CFestivalActivity>::instance();
  v14->Init(0);
  InitDropTimes();
  InitMoYuShiJieRecord();
  requestSocialData();
  requestWorldLevel();
  if ( this->m_line == 1 )
  {
    SendServerDiffToGlobal();
    v15 = Answer::Singleton<GameService>::instance();
    packet = popNetpacket(Answer::PackType::PACK_PROC, 0x4E30u);
    if ( packet )
    {
      v16 = Answer::Singleton<CfgData>::instance();
      int32_t Debug = v16->getDebug();
      packet->writeInt32(Debug);
  uint32_t wOffset = 0;
      wOffset = packet->getWOffset();
      packet->setSize(wOffset);
      v19 = Answer::Singleton<GameService>::instance();
      sendPacket(0, packet);
    }
  }
}


void GameService::stopGame()
{
  MapManager *v1; 
  GameService *v2; 

  v1 = Answer::Singleton<MapManager>::instance();
  v1->StopAll();
  v2 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v2) != 9 )
    saveAllPlayerToDB();
}


int8_t GameService::GetAutoConnId()
{
  const MutiConn<GameService> *pConn;

  pConn = GameService::GetConn(this);
  if ( pConn )
    return pConn->GetId();
  else
    return 0;
}


int8_t GameService::GetConnId(CharId_t cid)
{
  int8_t ConnId; 
  CharId_t cida;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  cida = cid;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter = this->m_players.find(cida);
  __x = this->m_players.end();
  if ( iter != __x
    && (pPlayer = iter->second) != 0 )
  {
    ConnId = Player::getConnId(pPlayer);
  }
  else
  {
    ConnId = 0;
  }
  /* MutexGuard destroyed */
  return ConnId;
}


void GameService::onUpdatePay()
{
  DBService *v1; 

  if ( this->m_line == 1 )
  {
    v1 = Answer::Singleton<DBService>::instance();
    v1->OnUpdatePay();
  }
}


void GameService::onNewMinuteCome(int32_t minute)
{
  DBService *v2; 

  std::pair<const int,ChatValidate> *v4; 
  std::pair<const int,ChatValidate> *v5; 
  std::_Rb_tree_iterator<std::pair<const int,ChatValidate> > v6; 
  std::_Rb_tree_iterator<std::pair<const int,ChatValidate> > it;
  std::_Rb_tree_iterator<std::pair<const int,ChatValidate> > __x;
  Answer::NetPacket *packet;
  if ( this->m_line == 1 )
  {
    v2 = Answer::Singleton<DBService>::instance();
    v2->onNewMinuteCome(minute);
    packet = popNetpacket(0, Answer::PackType::PACK_PROC, 0x4E38u);
    if ( !packet )
      return;
    packet->writeInt32(minute);
  uint32_t wOffset = 0;
    wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    sendPacket(0, packet);
  }
  if ( !(minute % 5) )
  {
    int32_t Time = Answer::DayTime::now();
    Answer::MutexGuard lock(&this->m_ChatValidateLock);
    it = this->m_ChatValidateMap.begin();
    while ( 1 )
    {
      __x = this->m_ChatValidateMap.end();
      if ( it == __x )
        break;
      v4 = it.operator->();
      if ( Time - v4->second.Time <= 300 )
      {
        ++it;
      }
      else
      {
        if ( it->second.SendPacket )
        {
          v5 = it.operator->();
          Answer::NetPacket::destroy(v5->second.SendPacket);
        }
        v6 = ++it;
        this->m_ChatValidateMap.erase(v6);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::OnDaySwitch()
{
  if ( this->m_line == 1 )
  {
    ResetDropTimes();
    SendServerDiffToGlobal();
  }
}


void GameService::onPlayerLoaded( PlayerDBData *const dbData, int32_t reason, bool isRobot)
{

  MapManager *v5; 
  CfgData *v7; 

  MapManager *v9; 
  PlayerRobot *v10; 
  CPoolManager *v11; 


  MapManager *v15; 
  CExtOperateLimit *OperateLimit; 

  CActivityManager *v18; 
  GameService *v19; 

  TencentInfo info;
  Player *player;
  Map *pMap;
  User *pUser;
  Map *pMap_0;
  CfgMapRegion *pCfgRegion;
  Map *pTempMap;
  Answer::NetPacket *packet;

  player = getPlayer(dbData->chr.data.cid, 0, 0);
  if ( player )
  {
    pMap_0 = StaticObj::getMap(player);
    int32_t mapid = 0;
    if ( pMap_0 )
      mapid = pMap_0->GetMapId();
    Answer::Logger::print(
      Answer::LogLevel::LOG_LEVEL_INFO,
      "GameService::onPlayerLoaded player cid = %lld, map = %p, mapid = %d already in game\n",
      dbData->chr.data.cid,
      pMap_0,
      mapid);
  }
  else
  {
    int32_t v4 = dbData->chr.data.mapid;
    v5 = Answer::Singleton<MapManager>::instance();
    pMap = v5->GetMap(v4);
    if ( !pMap )
    {
      int32_t nRegion = 20001;
      if ( dbData->chr.data.mapid > 0 )
      {
        if ( reason == 3 && GameService::getLine(this) == 9 )
          nRegion = 20002;
      }
      else
      {
        nRegion = 10002;
      }
      v7 = Answer::Singleton<CfgData>::instance();
      pCfgRegion = v7->getMapRegion(nRegion);
      if ( pCfgRegion )
      {
        int32_t v8 = pCfgRegion->mapid;
        v9 = Answer::Singleton<MapManager>::instance();
        pTempMap = v9->GetMap(v8);
        if ( pTempMap )
        {
          Position pos = pTempMap->getRandomWalkablePositionInRegion(pCfgRegion);
          if ( pos.x >= 0 && pos.y >= 0 )
          {
            pMap = pTempMap;
            dbData->chr.data.mapid = pTempMap->GetMapId();
            *(Position *)&dbData->chr.data.x = pos;
          }
        }
      }
    }
    if ( pMap )
    {
      Answer::MutexGuard lock(&this->m_userLock);
      int32_t nIndex = getUserIndex(dbData->connid, dbData->cgindex);
      pUser = this->m_users[nIndex];
      if ( pUser )
      {
        if ( User::getPlayer(pUser) )
        {
          Answer::Logger::print(
            Answer::LogLevel::LOG_LEVEL_ERROR,
            "GameService::onPlayerLoaded user already have player cgindex=%d, cid = %lld\n",
            dbData->cgindex,
            dbData->chr.data.cid);
        }
        else
        {
          if ( isRobot )
          {
            PlayerRobot *v10 = new PlayerRobot();
            player = v10;
            v10->init();
          }
          else
          {
            v11 = Answer::Singleton<CPoolManager>::instance();
            player = v11->pop<Player>();
          }
          if ( player )
          {
            User::GetTencentInfo(&info, pUser);
            Player::InitTencentInfo(player, &info);
            /* info auto-destructed */
            Player::init(player, dbData);
            User::setPlayer(pUser, player);
            AddPlayer(player, reason);
            if ( !Player::reconnect(player) )
            {
              int32_t iparam2 = dbData->chr.data.y;
              int32_t x = dbData->chr.data.x;
              int32_t RunnerId = pMap->GetRunnerId();
              v15 = Answer::Singleton<MapManager>::instance();
              v15->PostMsg(RunnerId, GameMsgCode::GMC_PLAYER_ENTER_MAP, player, pMap, x, iparam2, 0);
              if ( GameService::getLine(this) == 9 )
              {
                OperateLimit = player->GetOperateLimit();
                int32_t LimitCount = CExtOperateLimit::GetLimitCount(OperateLimit, 1057);
                v18 = Answer::Singleton<CActivityManager>::instance();
                int32_t nActId = v18->GetCurActivityId(LimitCount);
                int8_t connId = Player::getConnId(player);
                v19 = Answer::Singleton<GameService>::instance();
                packet = GameService::popNetpacket(v19, connId, Answer::PackType::PACK_DISPATCH, 0x1Du);
                if ( packet )
                {
                  packet->writeInt32(nActId);
  uint32_t wOffset = 0;
                  wOffset = packet->getWOffset();
                  packet->setSize(wOffset);
                  player->addNetPacket(packet, 0);
                  packet->destroy();
                }
              }
            }
            Answer::Logger::print(
              Answer::LogLevel::LOG_LEVEL_DEBUG,
              "GameService::onPlayerLoaded pop player %p\n",
              player);
          }
          else
          {
            Answer::Logger::print(
              Answer::LogLevel::LOG_LEVEL_ERROR,
              "GameService::onPlayerLoaded create player fail cgindex=%d, cid = %lld\n",
              dbData->cgindex,
              dbData->chr.data.cid);
          }
        }
      }
      else
      {
        Answer::Logger::print(
          Answer::LogLevel::LOG_LEVEL_ERROR,
          "GameService::onPlayerLoaded null user cgindex=%d, cid = %lld\n",
          dbData->cgindex,
          dbData->chr.data.cid);
      }
      /* MutexGuard destroyed */
    }
    else
    {
      Answer::Logger::print(
        Answer::LogLevel::LOG_LEVEL_INFO,
        "GameService::onPlayerLoaded player cid=%lld can't find map with mapid = %d\n",
        dbData->chr.data.cid,
        dbData->chr.data.mapid);
    }
  }
}


void GameService::onPlayerLogout(Player *player)
{
  CPoolManager *v2; 
  User *user;

  if ( player )
  {
    int8_t connid = Player::getConnId(player);
    int16_t cgindex = Player::getGateIndex(player);
    Answer::Logger::print(
      Answer::LogLevel::LOG_LEVEL_DEBUG,
      "GameService::onPlayerLogout push player %p, cgindex = %d\n",
      player,
      cgindex);
    removePlayer(player);
    if ( Player::getGateIndex(player) > 0 )
    {
      player->setGateIndex(-1);
      if ( (*((uint8_t (**)(Player *))player->_vptr_Entity + 34))(player) )
      {
        (*((void (**)(Player *))player->_vptr_Entity + 1))(player);
      }
      else
      {
        v2 = Answer::Singleton<CPoolManager>::instance();
        v2->Push(player);
      }
    }
    if ( (uint8_t)connid < 0x64u && (uint16_t)cgindex < 0x2710u )
    {
      Answer::MutexGuard lock(&this->m_userLock);
      int32_t nIndex = getUserIndex(connid, cgindex);
      user = this->m_users[nIndex];
      if ( user )
        User::setPlayer(user, 0);
      /* MutexGuard destroyed */
    }
  }
}


void GameService::onPlayerSaved( int8_t connid, int64_t uid, int32_t sid, int32_t reason, int32_t param, CharId_t cid)
{

  Answer::NetPacket *packet;

  packet = popNetpacket(connid, Answer::PackType::PACK_PROC, 0x4E25u);
  if ( packet )
  {
    packet->writeInt64(uid);
    packet->writeInt32(sid);
    packet->writeInt32(reason);
    packet->writeInt64(cid);
    packet->writeInt32(param);
  uint32_t wOffset = 0;
    wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    sendPacket(connid, packet);
  }
}


void GameService::sendPacket(int8_t connid, Answer::NetPacket *packet)
{
  ConnType *pConn;

  pConn = GetConn(connid);
  if ( pConn )
    pConn->sendPacket(packet);
}


void GameService::sendPacketTo( int8_t connid, int16_t index, Answer::NetPacket *inPacket)
{
  ConnType *pConn;

  pConn = GetConn(connid);
  if ( pConn )
    pConn->sendPacketTo(index, inPacket);
}


int32_t GameService::replySuccess( int8_t connid, int16_t index, uint16_t proc, int64_t addon)
{
  ConnType *pConn;

  pConn = GetConn(connid);
  if ( pConn )
    return pConn->replySuccess(index, proc, addon);
  else
    return 10002;
}


int32_t GameService::replyfailure( int8_t connid, int16_t index, uint16_t proc, int32_t errcode, int64_t addon)
{
  ConnType *pConn;

  pConn = GetConn(connid);
  if ( pConn )
    return pConn->replyfailure(index, proc, errcode, addon);
  else
    return 10002;
}


void GameService::broadcastHuoDongDaTingIcon()
{
  std::pair<const long int,Player*> *v1; 
  DailyActivity *PlayerDailyActivity; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  if ( !this->m_players.empty() )
  {
    for ( it = this->m_players.begin();
          ;
          ++it )
    {
      __x = this->m_players.end();
      if ( it == __x )
        break;
      v1 = it.operator->();
      PlayerDailyActivity = Player::GetPlayerDailyActivity(v1->second);
      DailyActivity::SendHuoDongDaTingIcon(PlayerDailyActivity);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::SendOpenBetaIcon()
{
  Player *second; 
  COpenBeta *v2; 
  std::pair<const long int,Player*> *v3; 
  ShowIcon stu;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  if ( !this->m_players.empty() )
  {
    for ( it = this->m_players.begin();
          ;
          ++it )
    {
      __x = this->m_players.end();
      if ( it == __x )
        break;
      second = it->second;
      v2 = Answer::Singleton<COpenBeta>::instance();
      COpenBeta::getIconState(&stu, v2, second);
      v3 = it.operator->();
      Player::SendIconState(v3->second, &stu);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::broadFamilyWarIcon()
{
  std::pair<const long int,Player*> *v1; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  if ( !this->m_players.empty() )
  {
    for ( it = this->m_players.begin();
          ;
          ++it )
    {
      __x = this->m_players.end();
      if ( it == __x )
        break;
      v1 = it.operator->();
      Player::SendFamilyWarIcon(v1->second);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::broadcast( Answer::NetPacket *inPacket, const IndexMap *const indexMap)
{
  std::list<short int> *p_second;   /* var */std::_Rb_tree_const_iterator<std::pair<const int8_t,std::list<short int> > > iter;
  std::_Rb_tree_const_iterator<std::pair<const int8_t,std::list<short int> > > __x;

  if ( inPacket )
  {
    if ( indexMap->empty() )
    {
      inPacket->destroy();
    }
    else
    {
      for ( iter = indexMap->begin();
            ;
            ++iter )
      {
        __x = indexMap->end();
        if ( iter == __x )
          break;
        p_second = &iter->second;
        int8_t first = iter->first;
        broadcast(inPacket, first, p_second);
      }
      inPacket->destroy();
    }
  }
}


void GameService::broadcast( Answer::NetPacket *inPacket, int8_t connid, const Int16List *const indexList)
{


  int16_t *v7; 

  char *Buffer; 

  std::_List_const_iterator<short int> iter;
  std::_List_const_iterator<short int> __x;
  Answer::NetPacket *packet;
  size_t nsize;
  int32_t v4;
  uint16_t Proc;
  uint8_t Type;
  int32_t Size;

  if ( inPacket )
  {
    if (      indexList->empty() )
    {
      inPacket->destroy();
    }
    else
    {
      nsize = indexList->size();
      v4 = 2 * (nsize + 1) + inPacket->getSize();
      Proc = inPacket->getProc();
      Type = inPacket->getType();
      packet = popNetpacket(connid, Type, Proc, v4);
      if ( packet )
      {
        packet->writeInt16(nsize);
        for ( iter =      indexList->begin();
              ;
              ++iter )
        {
          __x =      indexList->end();
          if ( iter == __x )
            break;
          v7 = (int16_t *)*iter;
          Answer::NetPacket::writeInt16(packet, *v7);
        }
        Size = inPacket->getSize();
        Buffer = inPacket->getBuffer();
        packet->write(Buffer, Size);
  uint32_t wOffset = 0;
        wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        sendPacket(connid, packet);
      }
    }
  }
}


void GameService::broadcast(Answer::NetPacket *inPacket)
{
  std::list<short int> *v2; 
  IndexMap indexMap;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __it;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > v7;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v8;
  int16_t __x;
  char __k;
  Player *player;

  if ( inPacket )
  {
    IndexMap indexMap;
    Answer::MutexGuard lock(&this->m_playerLock);
    __it = this->m_players.begin();
    iter = __it;
    while ( 1 )
    {
      v8 = this->m_players.end();
      v7 = v8;
      if ( iter == v7 )
        break;
      player = iter->second;
      if ( player )
      {
        __x = Player::getGateIndex(player);
        __k = Player::getConnId(player);
        std::list<int16_t>& v2 = indexMap[__k];
        v2.push_back(__x);
      }
      ++iter;
    }
    /* MutexGuard destroyed */
    broadcast(inPacket, &indexMap);
    /* indexMap destructed */;
  }
}


void GameService::broadcast(Answer::NetPacket *inPacket, const CharIdList *const cids)
{
  const int64_t *v3; 
  std::list<short int> *v4; 
  IndexMap indexMap;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > findIter;
  std::_List_const_iterator<long int> iter;
  std::_List_const_iterator<long int> v10;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  int16_t GateIndex;
  char __k;
  Player *player;

  if ( inPacket )
  {
    if (      cids->empty() )
    {
      inPacket->destroy();
    }
    else
    {
      IndexMap indexMap;
      for ( iter =      cids->begin(); ; ++iter )
      {
        v10 =      cids->end();
        if ( iter == v10 )
          break;
        Answer::MutexGuard lock(&this->m_playerLock);
        v3 = *iter;
        findIter = this->m_players.find(v3);
        __x = this->m_players.end();
        if ( findIter != __x )
        {
          player = findIter->second;
          if ( player )
          {
            GateIndex = Player::getGateIndex(player);
            __k = Player::getConnId(player);
            v4 = indexMap[__k];
            v4.push_back(GateIndex);
          }
        }
        /* MutexGuard destroyed */
      }
      broadcast(inPacket, &indexMap);
      /* indexMap destructed */;
    }
  }
}


void GameService::broadcastToFamily( Answer::NetPacket *inPacket, FamilyId_t nFamilyId, int32_t nLevel)
{
  std::list<short int> *v5; 
  IndexMap indexMap;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __it;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > v12;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v13;
  int16_t __x;
  char __k;
  Player *player;

  if ( inPacket )
  {
    if ( nFamilyId > 0 )
    {
      IndexMap indexMap;
      Answer::MutexGuard lock(&this->m_playerLock);
      __it = this->m_players.begin();
      iter = __it;
      while ( 1 )
      {
        v13 = this->m_players.end();
        v12 = v13;
        if ( iter == v12 )
          break;
        player = iter->second;
        if ( player
          && player->getFamilyId() == nFamilyId
          && (*((int (**)(Player *))player->_vptr_Entity + 9))(player) > nLevel )
        {
          __x = Player::getGateIndex(player);
          __k = Player::getConnId(player);
          v5 = indexMap[__k];
          v5.push_back(__x);
        }
        ++iter;
      }
      /* MutexGuard destroyed */
      broadcast(inPacket, &indexMap);
      /* indexMap destructed */;
    }
    else
    {
      inPacket->destroy();
    }
  }
}


void GameService::worldBroadcast(int8_t connid, Answer::NetPacket *inPacket)
{


  char *Buffer; 

  Answer::NetPacket *packet;
  int32_t oldSize;
  int32_t v3;
  uint16_t Proc;
  uint8_t Type;
  int32_t Size;

  if ( inPacket )
  {
    oldSize = inPacket->getSize();
    if ( inPacket->rightShift(2u) )
    {
      Answer::NetPacket::writeInt16(inPacket, -1);
      Answer::NetPacket::setSize(inPacket, oldSize + 2);
      sendPacket(connid, inPacket);
    }
    else
    {
      v3 = inPacket->getSize() + 2;
      Proc = inPacket->getProc();
      Type = inPacket->getType();
      packet = popNetpacket(connid, Type, Proc, v3);
      if ( packet )
      {
        Answer::NetPacket::writeInt16(packet, -1);
        Size = inPacket->getSize();
        Buffer = inPacket->getBuffer();
        packet->write(Buffer, Size);
  uint32_t wOffset = 0;
        wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        sendPacket(connid, packet);
        inPacket->destroy();
      }
    }
  }
}


void GameService::worldBroadcast(Answer::NetPacket *inPacket)
{


  char *Buffer; 


  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > iter;
  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > __x;
  ConnType *pConn;
  Answer::NetPacket *packet;
  int32_t Size;
  int32_t v3;
  uint8_t Type;
  uint16_t Proc;

  if ( inPacket )
  {
    for ( iter = this->m_mConn.begin();
          ;
          ++iter )
    {
      __x = this->m_mConn.end();
      if ( iter == __x )
        break;
      pConn = iter->second;
      if ( pConn )
      {
        Size = inPacket->getSize();
        packet = Answer::TcpClient::popNetpacket(pConn, Size + 2);
        if ( packet )
        {
          Answer::NetPacket::writeInt16(packet, -1);
          v3 = inPacket->getSize();
          Buffer = inPacket->getBuffer();
          packet->write(Buffer, v3);
  uint32_t wOffset = 0;
          wOffset = packet->getWOffset();
          packet->setSize(wOffset);
          Type = inPacket->getType();
          packet->setType(Type);
          Proc = inPacket->getProc();
          packet->setProc(Proc);
          pConn->sendPacket(packet);
        }
      }
    }
    inPacket->destroy();
  }
}


void GameService::broadcastActivityState(int32_t nId, int8_t nState)
{

  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > iter;
  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > __x;
  ConnType *pConn;
  Answer::NetPacket *packet;

  for ( iter = this->m_mConn.begin();
        ;
        ++iter )
  {
    __x = this->m_mConn.end();
    if ( iter == __x )
      break;
    pConn = iter->second;
    if ( pConn )
    {
      packet = pConn->popNetpacket();
      if ( packet )
      {
        packet->writeInt32(nId);
        packet->writeInt8(nState);
  uint32_t wOffset = 0;
        wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_DISPATCH);
        packet->setProc(0x4EC4u);
        pConn->sendPacket(packet);
      }
    }
  }
}


void GameService::AddPlayer(Player *player, int32_t reason)
{
  FamilyId_t FamilyId; 

  CActivityManager *v5; 
  CharId_t Cid; 


  int8_t v10; 
  DBService *v11; 
  int64_t __k;
  int8_t ConnId;
  int32_t LastLoginTime;
  CharId_t v9;
  std::string strName;

  if ( player )
  {
    Answer::MutexGuard lock(&this->m_playerLock);
    __k = Player::getCid(player);
    this->m_players[__k] = player;
    /* MutexGuard destroyed */
    if ( reason != 3 )
    {
      FamilyId = player->getFamilyId();
      ConnId = Player::getConnId(player);
      v5 = Answer::Singleton<CActivityManager>::instance();
      if ( FamilyId == v5->GetCityWarWinner(ConnId) && player->getFamilyPosition() == 3 )
      {
        Player::getName(nullptr, &strName);
        Cid = Player::getCid(player);
        broadcastCityWarWinnerLogin(Cid, &strName);
              }
    }
    sendSocialAddPlayer(player);
    Player::sendBasicInfo(player, reason);
    LastLoginTime = player->getLastLoginTime();
    v9 = Player::getCid(player);
    v10 = Player::getConnId(player);
    v11 = Answer::Singleton<DBService>::instance();
    v11->UpdateLoginTime(v10, v9, LastLoginTime);
  }
}


void GameService::removePlayer(Player *player)
{
  CharId_t Cid; 
  std::pair<const long int,Player*> *v3;   /* var */std::pair<const long int,Player*> *v5; 

  std::pair<const long int,Player*> *v7; 

  CharId_t v9;   /* var */std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  int64_t v4;
  int64_t v6;
  int64_t v8;
  int64_t v10;
  int64_t __x;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v14;

  if ( player )
  {
    Answer::MutexGuard lock(&this->m_playerLock);
    __x = Player::getCid(player);
    it = this->m_players.find(__x);
    v14 = this->m_players.end();
    if ( it == v14 )
    {
      Cid = Player::getCid(player);
      Answer::Logger::print(
        Answer::LogLevel::LOG_LEVEL_INFO,
        "GameService::removePlayer player = %p, cid = %lld not in m_players\n",
        player,
        Cid);
    }
    else
    {
      v3 = it.operator->();
      v4 = (*((int64_t (**)(Player *))v3->second->_vptr_Entity + 2))(v3->second);
      if ( v4 == (*((int64_t (**)(Player *))player->_vptr_Entity + 2))(player) )
      {
        this->m_players.erase(it);
        sendSocialRemovePlayer(player);
      }
      else
      {
        v5 = it.operator->();
        v6 = Player::getCid(v5->second);
        v7 = it.operator->();
        v8 = (*((int64_t (**)(Player *))v7->second->_vptr_Entity + 2))(v7->second);
        v9 = Player::getCid(player);
        v10 = (*((int64_t (**)(Player *))player->_vptr_Entity + 2))(player);
        Answer::Logger::print(
          Answer::LogLevel::LOG_LEVEL_INFO,
          "GameService::removePlayer player entityid = %lld, cid = %lld not equal with it->seconde entity id = %lld, cid =%lld\n",
          v10,
          v9,
          v8,
          v6);
      }
    }
    /* MutexGuard destroyed */
  }
}


bool GameService::SendChatValidate( Player *Owner, int32_t Channel, Answer::NetPacket *inPacket)
{  /* var */int8_t ConnId; 


  CharId_t v8; 

  char *Buffer; 


  ChatValidate *v13; 
  ChatValidate *v14; 
  ChatValidate *v15; 
  std::string Key;
  std::string Md5String;
  std::string Cid;
  std::string Sid;
  std::string Name;
  std::string PassPort;
  std::string v32;
  std::string __rhs;
  Answer::MD5 v35;
  Answer::NetPacket *packet;
  if ( !inPacket || !Owner )
    return 0;
  Answer::MutexGuard lock(&this->m_playerLock);
  Answer::MutexGuard lockChat(&this->m_ChatValidateLock);
  ConnId = Player::getConnId(Owner);
  packet = popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2ED8u);
  if ( packet )
  {
    std::string PassPort, Name, Sid, Cid, Platform, Md5String, Key;
    int32_t SidInt = Owner->getSid();
    Player::GetPassport(Owner, &PassPort);
    Player::getName(Owner, &Name);
    std::stringstream ss1;
    ss1 << SidInt;
    ss1 >> Sid;
    int64_t v8 = Player::getCid(Owner);
    std::stringstream ss2;
    ss2 << v8;
    ss2 >> Cid;
    Player::GetPlatform(Owner, &Platform);
    Md5String = PassPort + Sid + Cid + Platform;
    Answer::MD5 v35;
    std::string md5Result = v35.md5sum(Md5String);
    Key = md5Result;
    packet->writeInt8(1);
    packet->writeInt32(0);
    packet->writeUTF8(&Key);
    int32_t Size = inPacket->getSize();
    const char* Buffer = inPacket->getBuffer();
    packet->write(Buffer, Size);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t GateIndex = Player::getGateIndex(Owner);
    int8_t connid = Player::getConnId(Owner);
    sendPacketTo(connid, GateIndex, packet);
    v4 = 1;
                          }
  else
  {
    v4 = 0;
  }
  /* MutexGuard destroyed */
  /* MutexGuard destroyed */
  return v4;
}


void GameService::addPet(CObjPet *pPet)
{
  if ( pPet )
  {
    Answer::MutexGuard lock(&this->m_petsLock);
    int64_t __k = CObjPet::GetPetId(pPet);
    this->m_pets[__k] = pPet;
    /* MutexGuard destroyed */
  }
}


void GameService::removePet(CObjPet *pPet)
{
  if ( pPet )
  {
    Answer::MutexGuard lock(&this->m_petsLock);
    int64_t __x = CObjPet::GetPetId(pPet);
    this->m_pets.erase(__x);
    /* MutexGuard destroyed */
  }
}


void GameService::addMonster(Monster *monster)
{
  if ( monster )
  {
    Answer::MutexGuard lock(&this->m_monstersLock);
    int64_t __k = (*((int64_t (**)(Monster *))monster->_vptr_Entity + 2))(monster);
    this->m_monsters[__k] = monster;
    /* MutexGuard destroyed */
  }
}


void GameService::removeMonster(Monster *monster)
{
  if ( monster )
  {
    Answer::MutexGuard lock(&this->m_monstersLock);
    int64_t __x = (*((int64_t (**)(Monster *))monster->_vptr_Entity + 2))(monster);
    this->m_monsters.erase(__x);
    /* MutexGuard destroyed */
  }
}


void GameService::addTrailer(Trailer *trailer)
{
  if ( trailer )
  {
    Answer::MutexGuard lock(/* mutex member */);
    int64_t __k = (*((int64_t (**)(Trailer *))trailer->_vptr_Entity + 2))(trailer);
    this->m_trailers[__k] = trailer;
    /* MutexGuard destroyed */
  }
}


void GameService::removeTrailer(Trailer *trailer)
{
  if ( trailer )
  {
    Answer::MutexGuard lock(/* mutex member */);
    int64_t __x = (*((int64_t (**)(Trailer *))trailer->_vptr_Entity + 2))(trailer);
    this->m_trailers.erase(__x);
    /* MutexGuard destroyed */
  }
}


void GameService::saveAllPlayerToDB()
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;
  int64_t nNowTime;

  nNowTime = Answer::DayTime::now();
  Answer::MutexGuard lock(&this->m_playerLock);
  for ( it = this->m_players.begin();
        ;
        ++it )
  {
    __x = this->m_players.end();
    if ( it == __x )
      break;
    player = it->second;
    if ( player )
      player->onLogout(1, 0);
  }
  /* MutexGuard destroyed */
}


void GameService::onUserPayed(Answer::NetPacket *inPacket)
{
  std::pair<const long int,Player*> *v2; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  CharId_t Cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  if ( inPacket )
  {
    Cid = inPacket->readInt64();
    Answer::MutexGuard lock(&this->m_playerLock);
    it = this->m_players.find(Cid);
    __x = this->m_players.end();
    if ( it != __x )
    {
      if ( it->second )
      {
        v2 = it.operator->();
        Player::addNetPacket(v2->second, inPacket, 8u);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::onAddUser( ConnType *pConn, Answer::NetPacket *inPacket)
{
  CPoolManager *v3; 
  User *user;
  int16_t cgindex;
  int64_t uid;
  int32_t sid;
  int8_t connid;
  int32_t nIndex;

  if ( inPacket )
  {
    cgindex = inPacket->readInt16();
    uid = inPacket->readInt64();
    sid = inPacket->readInt32();
    connid = pConn->GetId();
    Answer::MutexGuard lock(&this->m_userLock);
    if ( (uint8_t)connid < 0x64u && cgindex > 0 && cgindex <= 9999 && uid > 0 && sid > 0 )
    {
      nIndex = getUserIndex(connid, cgindex);
      v3 = Answer::Singleton<CPoolManager>::instance();
      user = v3->pop<Player>();
      if ( user )
      {
        User::init(user, uid, sid);
        this->m_users[nIndex] = user;
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::qqToPlayer( ConnType *pConn, Answer::NetPacket *inPacket)
{
  TencentInfo info;
  User *user;
  int16_t cgindex;
  int8_t connid;
  int32_t nIndex;

  if ( inPacket )
  {
    cgindex = inPacket->readInt16();
    connid = pConn->GetId();
    TencentInfo info;
    info.UnPacketInfo(inPacket);
    Answer::MutexGuard lock(&this->m_userLock);
    nIndex = getUserIndex(connid, cgindex);
    user = this->m_users[cgindex];
    if ( user )
      User::SetTencentInfo(user, &info);
    /* MutexGuard destroyed */
    /* info auto-destructed */
  }
}


void GameService::onRemoveUser( ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t v3; 
  GameService *v4; 

  CharId_t Cid; 
  void (*v7)(Player *, uint64_t, uint64_t, uint64_t); 

  CPoolManager *v9; 
  CPoolManager *v10; 
  User *user;
  Player *player;
  Answer::NetPacket *packet;
  int16_t cgindex;
  int8_t connid;
  int32_t reason;
  int32_t param;
  int32_t Now;

  if ( inPacket )
  {
    cgindex = inPacket->readInt16();
    connid = pConn->GetId();
    if ( (uint8_t)connid < 0x64u && cgindex > 0 && cgindex <= 9999 )
    {
      Answer::MutexGuard lock(&this->m_userLock);
      int32_t nIndex = getUserIndex(connid, cgindex);
      user = this->m_users[nIndex];
      if ( user )
      {
        player = User::getPlayer(user);
        if ( !player )
        {
// label
          this->m_users[nIndex] = 0;
          User::reset(user);
          v10 = Answer::Singleton<CPoolManager>::instance();
          v10->push(user);
          // goto removed
        }
        inPacket->readInt64();
        reason = inPacket->readInt32();
        param = inPacket->readInt32();
        if ( !StaticObj::getMap(player) )
        {
          Cid = Player::getCid(player);
          Answer::Logger::print(
            Answer::LogLevel::LOG_LEVEL_INFO,
            "GameService::onRemoveUser remove player with m_pMap == NULL, cid = %lld\n",
            Cid);
          v7 = (void (*)(Player *, uint64_t, uint64_t, uint64_t))*((uint64_t *)player->_vptr_Entity + 35);
          Now = Unit::getNow(player);
          v7(player, (uint32_t)reason, (uint32_t)param, Now);
          removePlayer(player);
          if ( Player::getGateIndex(player) > 0 )
          {
            player->setGateIndex(-1);
            if ( (*((uint8_t (**)(Player *))player->_vptr_Entity + 34))(player) )
            {
              if ( player )
                (*((void (**)(Player *))player->_vptr_Entity + 1))(player);
            }
            else
            {
              v9 = Answer::Singleton<CPoolManager>::instance();
              v9->push(player);
            }
          }
          // goto removed
        }
        v3 = Player::getConnId(player);
        v4 = Answer::Singleton<GameService>::instance();
        packet = GameService::popNetpacket(v4, v3, Answer::PackType::PACK_DISPATCH, 8u);
        if ( packet )
        {
          packet->writeInt32(reason);
          packet->writeInt32(param);
  uint32_t wOffset = 0;
          wOffset = packet->getWOffset();
          packet->setSize(wOffset);
          player->addNetPacket(packet, 0);
          packet->destroy();
          // goto removed
        }
      }
// label
      /* MutexGuard destroyed */
    }
  }
}


void GameService::onGMReloadItem( ConnType *pConn, Answer::NetPacket *inPacket)
{
  CfgData *v3; 

  v3 = Answer::Singleton<CfgData>::instance();
  v3->reload();
}


void GameService::onSocialNetpacket( ConnType *pConn, Answer::NetPacket *inPacket)
{
  int32_t cgindex;
  int8_t connid;

  if ( inPacket )
  {
    cgindex = inPacket->readInt32();
    connid = pConn->GetId();
    if ( (uint8_t)connid < 0x64u && cgindex > 0 && cgindex <= 9999 )
      onGameNetpacket(connid, cgindex, inPacket);
  }
}


void GameService::onGateDisconnect( ConnType *pConn, Answer::NetPacket *inPacket)
{  /* var */Answer::NetPacket *packet;

  if ( pConn && inPacket )
  {
    std::ofstream v7("./log/GameConnErr.txt", std::ios::out | std::ios::trunc);
    int32_t v4 = Answer::DayTime::now();
    v7 << v4 << std::endl;
    packet = popNetpacket(Answer::PackType::PACK_PROC, 0x4E2Eu);
    if ( packet )
    {
      packet->writeInt32(1);
  uint32_t wOffset = 0;
      wOffset = packet->getWOffset();
      packet->setSize(wOffset);
      pConn->sendPacket(packet);
    }
    /* v7 auto-destructed */
  }
}


void GameService::onEnterGame( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{

  DBService *v5; 
  User *user;
  int64_t cid;
  int32_t line;
  int32_t reason;
  int64_t uid;
  int32_t sid;
  int32_t nIndex;
  int32_t v4;

  cid = inPacket->readInt64();
  line = inPacket->readInt32();
  reason = inPacket->readInt32();
  // LOG(LOG_LEVEL_DEBUG, "GameService::onEnterGame() cid=%lld, line=%d, reason=%d \n", cid, line, reason);
  uid = 0;
  sid = 0;
  nIndex = getUserIndex(connid, cgindex);
  Answer::MutexGuard lock(&this->m_userLock);
  user = this->m_users[nIndex];
  if ( user )
  {
    uid = User::getUid(user);
    sid = User::getSid(user);
    v4 = 1;
  }
  else
  {
    v4 = 0;
  }
  /* MutexGuard destroyed */
  if ( v4 )
  {
    v5 = Answer::Singleton<DBService>::instance();
    v5->loadPlayer(connid, cgindex, uid, sid, reason);
  }
}


void GameService::onEnterGameRobot( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{

  const char *v6; 
  Answer::Random *v7; 
  PlayerDBData dbData;
  Answer::NetPacket v13;
  int32_t id[3];
  int32_t v5;

  if ( inPacket )
  {
    /* *(int64_t*)&v13.m_wOffset */ = inPacket->readInt32();
    *(int32_t*)&v13.m_proc = getUserIndex(connid, cgindex);
    Answer::MutexGuard lock(&this->m_userLock);
    /* *(int64_t*)&v13.m_size */ = this->m_users[/* *(int*)&v13.m_proc */];
    if ( /* *(int64_t*)&v13.m_size */ && !User::getCid(*(const User *const *)&v13.m_size) )
    {
      User::setCid(*(User *const *)&v13.m_size, *(CharId_t *)&v13.m_wOffset);
      v5 = 1;
    }
    else
    {
      v5 = 0;
    }
    /* MutexGuard destroyed */
    if ( v5 )
    {
      if ( /* *(int64_t*)&v13.m_wOffset */ > 19088743 )
      {
        PlayerDBData dbData;
        inPacket->readInt32();
        dbData.chr.data.cid = /* *(int64_t*)&v13.m_wOffset */;
        Answer::NetPacket::readUTF8(&v13, (bool)inPacket);
        v6 = (const char *)std::string::c_str((std::string *)&v13);
        snprintf(dbData.chr.data.name, 0x1Eu, v6);
                dbData.chr.data.sex = inPacket->readInt32();
        dbData.chr.data.job = inPacket->readInt32();
        inPacket->readInt32();
        inPacket->readInt32();
        dbData.chr.data.level = 60;
        dbData.chr.data.mapid = inPacket->readInt32();
        dbData.chr.data.x = inPacket->readInt32();
        dbData.chr.data.y = inPacket->readInt32();
        dbData.chr.data.hp = inPacket->readInt32();
        dbData.chr.data.mp = inPacket->readInt32();
        dbData.chr.data.kun_li = inPacket->readInt32();
        dbData.chr.data.head = inPacket->readInt32();
        id[0] = 10001;
        id[1] = 10006;
        id[2] = 10011;
        v7 = Answer::Singleton<Answer::Random>::instance();
        v13.m_refCount = id[Answer::Random::generate(v7, 0, 0)];
        onPlayerLoaded(&dbData, 0, 0);
        /* dbData auto-destructed */
      }
    }
  }
}


Player* GameService::getUserPlayer(int8_t connid, int16_t cgindex)
{
  int32_t nIndex = getUserIndex(connid, cgindex);
  if ( nIndex < 0 || nIndex >= MAX_USERS )
    return nullptr;
  Answer::MutexGuard lock(&this->m_userLock);
  User *user = this->m_users[nIndex];
  if ( user )
    return User::getPlayer(user);
  return nullptr;
}


void GameService::onGameNetpacket( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{
  Player *player;

  if ( inPacket )
  {
    player = getUserPlayer(connid, cgindex);
    if ( player )
      player->addNetPacket(inPacket, 4u);
  }
}


void GameService::OnDBNetPacket( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{
  Player *player;

  if ( inPacket )
  {
    player = getUserPlayer(connid, cgindex);
    if ( player )
      player->addNetPacket(inPacket, 2u);
  }
}


void GameService::sendSocialAddPlayer(Player *player)
{
  int8_t ConnId; 

  CharId_t Cid;   /* var */int8_t GMLevel; 

  int8_t v9; 
  Answer::NetPacket *packet;
  int16_t GateIndex;
  int64_t Uid;
  int32_t Sid;

  if ( player )
  {
    ConnId = Player::getConnId(player);
    packet = popNetpacket(ConnId, Answer::PackType::PACK_PROC, 0x4E34u);
    if ( packet )
    {
      GateIndex = Player::getGateIndex(player);
      packet->writeInt32(GateIndex);
      Cid = Player::getCid(player);
      packet->writeInt64(Cid);
      Uid = player->getUid();
      packet->writeInt64(Uid);
      Sid = player->getSid();
      packet->writeInt32(Sid);
      Answer::NetPacket::writeInt32(packet, this->m_line);
      GMLevel = Player::GetGMLevel(player);
      packet->writeInt8(GMLevel);
  uint32_t wOffset = 0;
      wOffset = packet->getWOffset();
      packet->setSize(wOffset);
      v9 = Player::getConnId(player);
      sendPacket(v9, packet);
    }
  }
}


void GameService::sendSocialRemovePlayer(Player *player)
{
  int8_t ConnId; 
  CharId_t Cid; 

  int8_t v5; 
  Answer::NetPacket *packet;

  if ( player )
  {
    ConnId = Player::getConnId(player);
    packet = popNetpacket(ConnId, Answer::PackType::PACK_PROC, 0x4E35u);
    if ( packet )
    {
      Cid = Player::getCid(player);
      packet->writeInt64(Cid);
  uint32_t wOffset = 0;
      wOffset = packet->getWOffset();
      packet->setSize(wOffset);
      v5 = Player::getConnId(player);
      sendPacket(v5, packet);
    }
  }
}


void GameService::onSocialUpdatePetRank( ConnType *pConn, Answer::NetPacket *inPacket)
{
  CPetManager *v3; 

  v3 = Answer::Singleton<CPetManager>::instance();
  v3->OnUpdatePetRank(inPacket);
}


void GameService::onSocialUpdateFamilyInfo( ConnType *pConn, Answer::NetPacket *inPacket)
{
  FamilyManager *v3; 

  v3 = Answer::Singleton<FamilyManager>::instance();
  v3->OnUpdateFamilyInfo(inPacket);
}


void GameService::onSocialInitPetRank( ConnType *pConn, Answer::NetPacket *inPacket)
{
  CPetManager *v3; 

  v3 = Answer::Singleton<CPetManager>::instance();
  v3->OnInitPetRank(inPacket);
}


void GameService::onSocialInitRankMirror( ConnType *pConn, Answer::NetPacket *inPacket)
{
  CRankMirror *v3; 

  v3 = Answer::Singleton<CRankMirror>::instance();
  v3->OnSocialUpdateRank(inPacket);
}


void GameService::onSocialUpdateRankMirror( ConnType *pConn, Answer::NetPacket *inPacket)
{
  CRankMirror *v3; 

  v3 = Answer::Singleton<CRankMirror>::instance();
  v3->OnSocialUpdateRank(inPacket);
}


void GameService::onSocialQueryPlayerInfo( ConnType *pConn, Answer::NetPacket *inPacket)
{

  int8_t Id; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  CharId_t cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;
  int16_t cgindex;
  int16_t v3;

  if ( pConn && inPacket )
  {
    cid = inPacket->readInt64();
    cgindex = inPacket->readInt16();
    Answer::MutexGuard lock(&this->m_playerLock);
    iter = this->m_players.find(cid);
    __x = this->m_players.end();
    if ( iter != __x )
    {
      pPlayer = iter->second;
      if ( pPlayer )
      {
        v3 = cgindex;
        Id = pConn->GetId();
        pPlayer->SendOtherPlayerInfo(Id, v3);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::onSocialCreateTeamDungeon( ConnType *pConn, Answer::NetPacket *inPacket)
{
  MapManager *v3; 

  MapManager *v5; 
  CharId_t memberId;
  CharIdList memberList;
  Dungeon *pDungeon;
  int32_t nDungeonId;
  int32_t i;
  int32_t RunnerId;

  nDungeonId = inPacket->readInt32();
  v3 = Answer::Singleton<MapManager>::instance();
  pDungeon = v3->NewDungeon(nDungeonId);
  if ( pDungeon )
  {
    CharIdList memberList;
    for ( i = 0; i <= 3; ++i )
    {
      memberId = inPacket->readInt64();
      if ( memberId > 0 )
        memberList.push_back(memberId);
    }
    Dungeon::InitTeamMember(pDungeon, &memberList);
    pDungeon->start(0);
    RunnerId = pDungeon->GetRunnerId();
    v5 = Answer::Singleton<MapManager>::instance();
    v5->PostMsg(RunnerId, GameMsgCode::GMC_ADD_DUNGEON, pDungeon, 0, 0, 0, 0);
    /* memberList destructed */;
  }
}


void GameService::onSocialSendSystemMail( ConnType *pConn, Answer::NetPacket *inPacket)
{

  DBService *v4; 
  DBService *v5; 
  MemChrBag item;
  std::string mailParam;
  int64_t nReceiver;
  int32_t nMailId;
  int32_t nReason;
  int32_t v3;

  if ( pConn && inPacket )
  {
    nReceiver = inPacket->readInt64();
    nMailId = inPacket->readInt32();
    nReason = inPacket->readInt32();
    *(&item.endTime + 1) = 0;
    item.itemId = inPacket->readInt32();
    *(int32_t*)&item.itemClass = (uint8_t)inPacket->readInt8();
    item.itemCount = inPacket->readInt32();
    item.srcId = inPacket->readInt64();
    *(int32_t*)&item.bind = (uint8_t)inPacket->readInt8();
    item.endTime = inPacket->readInt32();
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&mailParam, (bool)inPacket);
    if ( item.itemId <= 0 || item.itemCount <= 0 )
    {
      v5 = Answer::Singleton<DBService>::instance();
      v5->OnSendSysMail( 0, nReceiver, nMailId, &mailParam, 0);
    }
    else
    {
      v3 = nReason;
      v4 = Answer::Singleton<DBService>::instance();
      v4->OnSendSysMail( 0, nReceiver, nMailId, &item, (ITEM_CHANGE_REASON)v3, &mailParam, 0);
    }
      }
}


void GameService::onSocialSendSystemMail2( ConnType *pConn, Answer::NetPacket *inPacket)
{
  MemChrBag *v3; 
  MemChrBag *v4; 
  MemChrBag *v5; 

  DBService *v7; 
  DBService *v8; 
  MemChrBagVector vItem;
  std::string Param;
  std::string v15;
  int64_t nReceiver;
  int32_t nMailId;
  int32_t nReason;
  int32_t nSize;
  MemChrBag v12;
  MemChrBag __x;
  int32_t i;
  int32_t v6;

  if ( pConn && inPacket )
  {
    nReceiver = inPacket->readInt64();
    nMailId = inPacket->readInt32();
    nReason = inPacket->readInt32();
    nSize = inPacket->readInt32();
    if ( nSize <= 0 )
    {
                  v8 = Answer::Singleton<DBService>::instance();
      v8->OnSendSysMail( 0, nReceiver, nMailId, &v15, 0);
                }
    else
    {
      std::vector<MemChrBag> vItem;
v12.itemId = 0;
      v12.itemClass = 0;
      v12.itemCount = 0;
      v12.bind = 0;
      v12.endTime = 0;
      v12.srcId = 0;
      __x.itemId = v12.itemId;
      __x.endTime = v12.endTime;
      vItem.resize(nSize, __x);
      for ( i = 0; i < nSize; ++i )
      {
        v3 = vItem[i];
        v3->itemClass = inPacket->readInt8();
        v4 = vItem[i];
        v4->itemId = inPacket->readInt32();
        v5 = vItem[i];
        v5->itemCount = inPacket->readInt32();
        vItem[i]->srcId = 0;
        vItem[i]->bind = 0;
        vItem[i]->endTime = 0;
      }
                  v6 = nReason;
      v7 = Answer::Singleton<DBService>::instance();
      v7->OnSendSysMail( 0, nReceiver, nMailId, &vItem, (ITEM_CHANGE_REASON)v6, &Param, 0);
                  /* vItem destructed automatically */
    }
  }
}


void GameService::onSetWorldLevel( ConnType *pConn, Answer::NetPacket *inPacket)
{

  if ( pConn )
  {
    if ( inPacket )
    {
      int32_t Int32 = inPacket->readInt32();
      setWorldLevel(Int32);
    }
  }
}


int32_t GameService::GetDropTimes(int32_t nGroupId)
{

  int32_t nGroupIda;

  std::_Rb_tree_iterator<std::pair<const int,int> > iter;
  std::_Rb_tree_iterator<std::pair<const int,int> > __x;

  nGroupIda = nGroupId;
  Answer::MutexGuard lock(&this->m_dropControlLock);
  iter = (std::map<int.find(int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda);
  __x = this->m_dropControl.end();
  int32_t second;
  if ( iter != __x )
    second = iter->second;
  else
    second = 0;
  /* MutexGuard destroyed */
  return second;
}


void GameService::AddDropTimes(int32_t nGroupId, int32_t nAddTimes)
{
  std::pair<const int,int> *v3; 
  GameService *v4; 
  CTimer *v5; 
  Answer::DBPool *v6; 

  DBService *v8; 
  int32_t nGroupIda;

  std::_Rb_tree_iterator<std::pair<const int,int> > iter;
  std::_Rb_tree_iterator<std::pair<const int,int> > __x;

  nGroupIda = nGroupId;
  int32_t nTimes = nAddTimes;
  Answer::MutexGuard lock(&this->m_dropControlLock);
  iter = (std::map<int.find(int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda);
  __x = this->m_dropControl.end();
  if ( iter != __x )
  {
    v3 = iter.operator->();
    v3->second += nAddTimes;
    nTimes = iter->second;
  }
  else
  {
    *(std::map<int[int> *const]((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda) = nAddTimes;
  }
  /* MutexGuard destroyed */
  v4 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v4) == 9 )
  {
    v5 = Answer::Singleton<CTimer>::instance();
    int32_t nowTime = v5->GetNow();
    v6 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(v6);
char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(
      szSQL,
      0xFFFu,
      "INSERT INTO `cross_drop_control` ( `id`, `group_id`, `times`, `time`) VALUES (%d,%d,%d,%d)  ON DUPLICATE KEY UPDAT"
      "E `times`=%d,`time`=%d",
      this->m_id,
      nGroupIda,
      nTimes,
      nowTime,
      nTimes,
      nowTime);
    db.excute(szSQL);
    /* db auto-destructed */
  }
  else
  {
    v7 = nGroupIda;
    v8 = Answer::Singleton<DBService>::instance();
    v8->SaveDropTimes(v7, nTimes);
  }
}


void GameService::UpdateDropTimes(int32_t nGroupId, int32_t nTimes)
{
  GameService *v3; 
  int32_t nGroupIda;


  nGroupIda = nGroupId;
  v3 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v3) != 9 )
  {
    Answer::MutexGuard lock(&this->m_dropControlLock);
    *(std::map<int[int> *const]((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda) = nTimes;
    /* MutexGuard destroyed */
  }
}


void GameService::ResetDropTimes()
{
  GameService *v1; 
  Answer::DBPool *v2; 
  DBService *v3; 

  v1 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v1) == 9 )
  {
    v2 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(v2);
char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, 0xFFFu, "DELETE FROM `cross_drop_control` WHERE `id`=%d", this->m_id);
    db.excute(szSQL);
    /* db auto-destructed */
  }
  else
  {
    v3 = Answer::Singleton<DBService>::instance();
    v3->ClearDropTimes();
  }
  Answer::MutexGuard lock(&this->m_dropControlLock);
  this->m_dropControl.clear();
  /* MutexGuard destroyed */
}


void GameService::InitDropTimes()
{
  Answer::DBPool *v1; 
  GameService *v2; 


  Answer::MySqlQuery *v7; 
  int *v8; 
  int32_t nGroupId;
  v1 = Answer::Singleton<Answer::DBPool>::instance();
  Answer::MySqlDBGuard db(v1);
  memset(szSQL, 0, sizeof(szSQL));
  v2 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v2) == 9 )
  {
    v3 = Answer::DayTime::now();
    v4 = Answer::DayTime::dayzero(v3);
    snprintf(szSQL, 0xFFFu, "SELECT * FROM `cross_drop_control` WHERE `id`=%d AND `time`>=%d", this->m_id, v4);
  }
  else
  {
    v5 = Answer::DayTime::now();
    v6 = Answer::DayTime::dayzero(v5);
    snprintf(szSQL, 0xFFFu, "SELECT * FROM `mem_drop_control` WHERE `time`>=%d", v6);
  }
  v7 = db.query(szSQL);
  Answer::MySqlQuery result(v7);
  while ( !result.eof() )
  {
    nGroupId = result.getIntValue("group_id", 0);
    nTimes = result.getIntValue("times", 0);
    v8 = (std::map<int[int> *const]((char *)&loc_7A13D0 + (int64_t)this), &nGroupId);
    *v8 = nTimes;
    result.nextRow();
  }
  /* result auto-destructed */
  /* db auto-destructed */
}


void GameService::TeamDungeonEnterDungeon( Dungeon *pDungeon, const CharIdList *const lst)
{

  CfgData *v4; 
  const int64_t *v5; 


  CHuoYueDu *PlayerHuoYueDu; 


  GameService *v13; 
  void (*v14)(Dungeon *, Player *, int64_t, uint64_t);   /* var */std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  std::_List_const_iterator<long int> eiter;
  std::_List_const_iterator<long int> iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  CfgDungeon *pCfgDungeon;
  Player *pPlayer;
  Map *pMap;
  int64_t DungeonId;
  int32_t DungeonGroupId;
  int32_t v7;
  int32_t v8;
  int32_t v9;
  int64_t v11;
  int16_t GateIndex;
  int32_t y;
  uint32_t x;

  DungeonId = pDungeon->getDungeonId();
  v4 = Answer::Singleton<CfgData>::instance();
  pCfgDungeon = v4->getDungeon(DungeonId);
  if ( pDungeon && pCfgDungeon )
  {
    iter = lst.begin();
    eiter = lst.end();
    while ( iter != eiter )
    {
      Answer::MutexGuard lock(&this->m_playerLock);
      v5 = *iter;
      it = this->m_players.find(v5);
      __x = this->m_players.end();
      if ( it != __x )
      {
        pPlayer = it->second;
        if ( pPlayer )
        {
          Player::setOldPosition(pPlayer);
          pMap = StaticObj::getMap(pPlayer);
          if ( pMap )
          {
            pPlayer->broadcastLeave();
            (*((void (**)(Map *, Player *, uint64_t))pMap->_vptr_Map + 16))(pMap, pPlayer, 0);
          }
          DungeonGroupId = pDungeon->getDungeonGroupId();
          v7 = pPlayer->getRecord(DungeonGroupId) + 1;
          v8 = pDungeon->getDungeonGroupId();
          pPlayer->updateRecord(v8, v7);
          v9 = pDungeon->getDungeonGroupId();
          PlayerHuoYueDu = pPlayer->GetPlayerHuoYueDu();
          CHuoYueDu::AddHuoYueDuRecord(PlayerHuoYueDu, 2, v9, 0);
          v11 = pDungeon->getDungeonId();
          GateIndex = Player::getGateIndex(pPlayer);
          int8_t connId = Player::getConnId(pPlayer);
          v13 = Answer::Singleton<GameService>::instance();
          v13->replySuccess(connId, GateIndex, 0x12u, v11);
          v14 = (void (*)(Dungeon *, Player *, int64_t, uint64_t))*((uint64_t *)pDungeon->_vptr_Map + 15);
          y = pDungeon->GetCfgDungeon()->y;
          x = (uint32_t)pDungeon->GetCfgDungeon()->x;
          v14(pDungeon, pPlayer, x, y);
        }
      }
      /* MutexGuard destroyed */
      ++iter;
    }
  }
}


void GameService::OnUpdateMail(Answer::NetPacket *inPacket)
{
  std::pair<const long int,Player*> *v2; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  CharId_t Cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  if ( inPacket )
  {
    Cid = inPacket->readInt64();
    Answer::MutexGuard lock(&this->m_playerLock);
    it = this->m_players.find(Cid);
    __x = this->m_players.end();
    if ( it != __x )
    {
      if ( it->second )
      {
        v2 = it.operator->();
        Player::addNetPacket(v2->second, inPacket, 8u);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::requestSocialData()
{

  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > iter;
  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > __x;
  ConnType *pConn;
  Answer::NetPacket *packet;

  for ( iter = this->m_mConn.begin();
        ;
        ++iter )
  {
    __x = this->m_mConn.end();
    if ( iter == __x )
      break;
    pConn = iter->second;
    if ( pConn )
    {
      packet = pConn->popNetpacket();
      if ( packet )
      {
        Answer::NetPacket::writeInt32(packet, this->m_line);
  uint32_t wOffset = 0;
        wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        packet->setProc(0x4E42u);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_DISPATCH);
        pConn->sendPacket(packet);
      }
    }
  }
}


void GameService::OnReceiveDaTiRank( ConnType *pConn, Answer::NetPacket *inPacket)
{
  GameService *v3; 
  int8_t v4; 
  DBService *v5; 
  GameService *v6; 


  int8_t v9; 
  DBService *v10; 
  int8_t v11; 
  DBService *v12; 
  int8_t v13; 
  DBService *v14; 
  int8_t v15; 
  DBService *v16; 
  int8_t v17; 
  DBService *v18; 
  int8_t v19; 
  CFestivalDoubleEleven *v20; 
  CTimer *v21; 
  int8_t v22; 
  DBService *v23; 
  LogActivity logActivity;
  std::string Name;
  std::string Param;
  std::string v29;
  std::string v31;
  std::string v33;
  std::string v35;
  std::string v37;
  Answer::NetPacket *packet;
  int32_t nSize;
  int32_t i;
  int64_t CharId;
  int32_t Index;
  int8_t connid;
  int32_t v7;

  if ( pConn && inPacket )
  {
    nSize = inPacket->readInt32();
    for ( i = 0; ; ++i )
    {
      if ( i >= nSize )
        return;
      CharId = inPacket->readInt64();
      Answer::NetPacket::readUTF8((Answer::NetPacket *const)&Name, (bool)inPacket);
      Index = inPacket->readInt32();
      v3 = Answer::Singleton<GameService>::instance();
      connid = GameService::GetConnId(v3, CharId);
      switch ( Index )
      {
        case 1:
                              v4 = connid;
          v5 = Answer::Singleton<DBService>::instance();
          v5->OnSendSysMail( v4, CharId, 6203, &Param, 0);
                              v6 = Answer::Singleton<GameService>::instance();
          packet = GameService::popNetpacket(v6, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
          if ( !packet )
          {
            v7 = 0;
            // goto removed
          }
          packet->writeInt32(28);
          Answer::NetPacket::writeUTF8(packet, &Name);
          packet->writeInt64(CharId);
  uint32_t wOffset = 0;
          wOffset = packet->getWOffset();
          packet->setSize(wOffset);
          worldBroadcast(0, packet);
          break;
        case 2:
                              v9 = connid;
          v10 = Answer::Singleton<DBService>::instance();
          v10->OnSendSysMail( v9, CharId, 6204, &v29, 0);
                              break;
        case 3:
                              v11 = connid;
          v12 = Answer::Singleton<DBService>::instance();
          v12->OnSendSysMail( v11, CharId, 6205, &v31, 0);
                              break;
        default:
          if ( Index > 10 )
          {
            if ( Index > 20 )
            {
                                          v17 = connid;
              v18 = Answer::Singleton<DBService>::instance();
              v18->OnSendSysMail( v17, CharId, 6206, &v37, 0);
                                        }
            else
            {
                                          v15 = connid;
              v16 = Answer::Singleton<DBService>::instance();
              v16->OnSendSysMail( v15, CharId, 6269, &v35, 0);
                                        }
          }
          else
          {
                                    v13 = connid;
            v14 = Answer::Singleton<DBService>::instance();
            v14->OnSendSysMail( v13, CharId, 6268, &v33, 0);
                                  }
          break;
      }
      v19 = connid;
      v20 = Answer::Singleton<CFestivalDoubleEleven>::instance();
      v20->SendDaTiReward(v19, Index, CharId);
      /* *(int64_t*)&logActivity.actid */ = 0x500000000LL;
      /* *(int64_t*)&logActivity.time */ = 0;
      logActivity.param = 0;
      logActivity.cid = CharId;
      v21 = Answer::Singleton<CTimer>::instance();
      logActivity.time = v21->GetNow();
      logActivity.param = Index;
      v22 = connid;
      v23 = Answer::Singleton<DBService>::instance();
      DBService::InsertActivityLog(v23, v22, &logActivity);
      v7 = 1;
// label
            if ( !v7 )
        return;
    }
  }
}


void GameService::OnReceiveDaTiResult( ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t v3; 
  std::pair<const long int,Player*> *v4; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  int64_t Cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  if ( pConn && inPacket )
  {
    Cid = inPacket->readInt64();
    int32_t Index = inPacket->readInt32();
    int8_t Result = inPacket->readInt8();
    Answer::MutexGuard lock(&this->m_playerLock);
    it = this->m_players.find(Cid);
    __x = this->m_players.end();
    if ( it != __x )
    {
      v3 = Result;
      v4 = it.operator->();
      Player::AddDaTiExp(v4->second, Index, v3);
    }
    /* MutexGuard destroyed */
  }
}


void GameService::SendServerDiffToGlobal()
{
  CfgData *v1; 


  Answer::NetPacket *packet;

  packet = popNetpacket(0, Answer::PackType::PACK_PROC, 0x4E49u);
  if ( packet )
  {
    v1 = Answer::Singleton<CfgData>::instance();
    int32_t ServerDiffDay = CfgData::getServerDiffDay(v1, SERVER_TYPE::SVT_NORMAL);
    packet->writeInt32(ServerDiffDay);
  uint32_t wOffset = 0;
    wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    sendPacket(0, packet);
  }
}


void GameService::UpdateCityWarTitle(FamilyId_t OldFamilyId, FamilyId_t NewFamilyId)
{
  std::pair<const long int,Player*> *v3; 
  std::pair<const long int,Player*> *v4; 
  CExtCharTitle *CharTitle; 
  std::pair<const long int,Player*> *v6; 
  std::pair<const long int,Player*> *v7; 

  std::pair<const long int,Player*> *v9; 
  CExtCharTitle *v10; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    v3 = iter.operator->();
    if ( Player::getFamilyId(v3->second) == OldFamilyId )
    {
      v4 = iter.operator->();
      CharTitle = Player::GetCharTitle(v4->second);
      if (CharTitle) CharTitle->RemoveTitle(1, 0);
    }
    v6 = iter;
    if ( Player::getFamilyId(v6->second) == NewFamilyId )
    {
      v7 = iter.operator->();
      int32_t FamilyPosition = Player::getFamilyPosition(v7->second);
      v9 = iter.operator->();
      v10 = Player::GetCharTitle(v9->second);
      if (v10) v10->CheckAddTitle(1, FamilyPosition);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::UpdateCityActState(FamilyId_t FamilyId, int8_t ActState)
{
  std::pair<const long int,Player*> *v3; 
  std::pair<const long int,Player*> *v4; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    v3 = iter.operator->();
    if ( Player::getFamilyId(v3->second) == FamilyId )
    {
      v4 = iter.operator->();
      Player::SetActState(v4->second, ActState);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::SetFamilyMemberNeedSync(FamilyId_t nFamilyId)
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    player = iter->second;
    if ( player && player->getFamilyId() == nFamilyId )
      player->SetNeedSyncAround();
  }
  /* MutexGuard destroyed */
}


void GameService::GetFamilyMemberInMap( FamilyId_t nFamilyId, int32_t nMapId, PlayerList *const pList, CharIdList *const farList, int32_t nLevel)
{
  Player *player;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v14;
  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    v14 = this->m_players.end();
    if ( iter == v14 )
      break;
    player = iter->second;
    if ( player
      && player->getFamilyId() == nFamilyId
      && (*((int (**)(Player *))player->_vptr_Entity + 9))(player) > nLevel )
    {
      if ( StaticObj::getMapId(player) == nMapId )
      {
        pList.push_back(player);
      }
      else
      {
        CharId_t __x = Player::getCid(player);
        farList.push_back(__x);
      }
    }
  }
  /* MutexGuard destroyed */
}


void GameService::onUpdateActivityState( ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t Int8; 
  CActivityManager *v4; 
  if ( pConn )
  {
    if ( inPacket )
    {
      int32_t nId = inPacket->readInt32();
      Int8 = inPacket->readInt8();
      v4 = Answer::Singleton<CActivityManager>::instance();
      v4->UpdateActivityState(nId, Int8);
    }
  }
}


void GameService::OnSocialCityMasterBanChat( ConnType *pConn, Answer::NetPacket *inPacket)
{
  int *v3; 
  CharId_t nCharId;
  if ( pConn )
  {
    if ( inPacket )
    {
      nCharId = inPacket->readInt64();
      int32_t nEndTime = inPacket->readInt32();
      Answer::MutexGuard lock(&this->m_cityMasterBanChatLock);
      v3 = this->m_mCityMasterBanChatMap[nCharId];
      *v3 = nEndTime;
      /* MutexGuard destroyed */
    }
  }
}


int32_t GameService::GetCityMasterBanChatTime(CharId_t nCharId)
{

  CharId_t nCharIda;


  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_cityMasterBanChatLock);
  int32_t v2 = this->m_mCityMasterBanChatMap[nCharIda];
  /* MutexGuard destroyed */
  return v2;
}


void GameService::KickUser(CharId_t cid, int32_t opWay)
{
  int8_t ConnId;   /* var */int8_t v7; 
  CharId_t cida;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;
  Answer::NetPacket *packet;

  cida = cid;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter = this->m_players.find(cida);
  __x = this->m_players.end();
  if ( !iter == __x )
  {
    player = iter->second;
    if ( player )
    {
      ConnId = Player::getConnId(player);
      packet = popNetpacket(ConnId, Answer::PackType::PACK_PROC, 0x4E2Au);
      if ( packet )
      {
        Uid = player->getUid();
        packet->writeInt64(Uid);
        Sid = player->getSid();
        packet->writeInt32(Sid);
        packet->writeInt32(opWay);
  uint32_t wOffset = 0;
        wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        v7 = Player::getConnId(player);
        sendPacket(v7, packet);
      }
    }
  }
  /* MutexGuard destroyed */
}


void GameService::KickAll(int32_t opWay)
{

  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > iter;
  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > __x;
  ConnType *pConn;
  Answer::NetPacket *packet;

  for ( iter = this->m_mConn.begin();
        ;
        ++iter )
  {
    __x = this->m_mConn.end();
    if ( iter == __x )
      break;
    pConn = iter->second;
    if ( pConn )
    {
      packet = pConn->popNetpacket();
      if ( packet )
      {
        packet->writeInt32(opWay);
  uint32_t wOffset = 0;
        wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
        packet->setProc(0x4E2Bu);
        pConn->sendPacket(packet);
      }
    }
  }
}


void GameService::onCheckTitle(CharId_t nCharId, int8_t nType, int32_t nParam)
{
  CExtCharTitle *CharTitle; 
  CharId_t nCharIda;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter = this->m_players.find(nCharIda);
  __x = this->m_players.end();
  if ( !iter == __x )
  {
    player = iter->second;
    if ( player )
    {
      CharTitle = player->GetCharTitle();
      if (CharTitle) CharTitle->CheckAddTitle(nType, nParam);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::onCheckTeShuTitle(CharId_t nCharId, int8_t nType, int32_t nParam)
{
  Answer::DBPool *v4; 

  CExtCharTitle *CharTitle; 
  CharId_t nCharIda;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter = this->m_players.find(nCharIda);
  __x = this->m_players.end();
  if ( iter == __x )
  {
    v4 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(v4);
    int32_t Time = Answer::DayTime::now() + 86340;
    char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    bzero(szSQL, 0x1000u);
    snprintf(
      szSQL,
      0xFFFu,
      "INSERT `mem_char_operate_limit`(cid,LimitId,LimitCount) VALUES (%lld,%d,%d) ON DUPLICATE KEY UPDATE LimitCount=%d",
      nCharIda,
      nParam + 31301,
      Time,
      Time);
    db.excute(szSQL);
    /* db auto-destructed */
  }
  else
  {
    player = iter->second;
    if ( player )
    {
      int32_t Now = Unit::getNow(player);
      Player::updateRecord(player, nParam + 31301, Now + 86340);
      CharTitle = player->GetCharTitle();
      if (CharTitle) CharTitle->CheckAddTitle(nType, nParam);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::onRemoveTitle(CharId_t nCharId, int8_t nType)
{
  CExtCharTitle *CharTitle; 
  CharId_t nCharIda;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter = this->m_players.find(nCharIda);
  __x = this->m_players.end();
  if ( !iter == __x )
  {
    player = iter->second;
    if ( player )
    {
      CharTitle = player->GetCharTitle();
      if (CharTitle) CharTitle->RemoveTitle(nType, 0);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::requestWorldLevel()
{

  Answer::NetPacket *packet;

  packet = popNetpacket(0, Answer::PackType::PACK_PROC, 0x4E4Bu);
  if ( packet )
  {
    packet->writeInt32(0);
  uint32_t wOffset = 0;
    wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    sendPacket(0, packet);
  }
}


void GameService::onRequestDropRecord( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

  int8_t Id; 
  CWorldBoss *v5; 

  if ( pConn )
  {
    if ( inPacket )
    {
      Int16 = inPacket->readInt16();
      Id = pConn->GetId();
      v5 = Answer::Singleton<CWorldBoss>::instance();
      CWorldBoss::SendDropRecord(v5, Id, Int16);
    }
  }
}


void GameService::InitServerBattle()
{
  GameService *v1; 
  DBService *v2; 
  Answer::DBPool *v3; 
  Answer::MySqlQuery *v4; 
  const char *StringValue; 

  v1 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v1) == 9 )
  {
    v2 = Answer::Singleton<DBService>::instance();
    DBService::onLoadServerBattle(v2);
  }
  else
  {
    v3 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(v3);
    v4 = Answer::MySqlDBGuard::query(&db, "SELECT * FROM `sys_server_config` WHERE `name`='BATTLE'");
    Answer::MySqlQuery result(v4);
    if ( !result.eof() )
    {
      StringValue = Answer::MySqlQuery::getStringValue(&result, "value", "");
      this->m_Battle = atoi(StringValue);
    }
    /* result auto-destructed */
    /* db auto-destructed */
  }
}


void GameService::SetServerBattle(int32_t Battle)
{
  if ( this->m_Battle < Battle )
    this->m_Battle = Battle;
}


void GameService::InitMoYuShiJieRecord()
{
  GameService *v1; 
  Answer::DBPool *v2; 
  Answer::MySqlQuery *v3; 
  const char *StringValue; 
  Answer::MySqlQuery *v5; 
  const char *v6; 
  LogMoYuShiJieReward record;
  v1 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v1) != 9 )
  {
    v2 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(v2);
    nSize = 0;
    v3 = Answer::MySqlDBGuard::query(
           &db,
           "SELECT * FROM `log_moyushijie_reward` WHERE `special`=1 ORDER BY `time` DESC LIMIT 5");
    Answer::MySqlQuery result(v3);
    while ( !result.eof() )
    {
      memset(&record, 0, sizeof(record));
      record.cid = Answer::MySqlQuery::getInt64Value(&result, "cid", 0);
      StringValue = Answer::MySqlQuery::getStringValue(&result, "name", "");
      snprintf(record.name, 0x1Eu, StringValue);
      record.dungeonid = result.getIntValue("dungeonid", 0);
      record.itemid = result.getIntValue("itemid", 0);
      record.itemclass = result.getIntValue("itemclass", 0);
      record.itemcount = result.getIntValue("itemcount", 0);
      record.time = result.getIntValue("time", 0);
      record.special = result.getIntValue("special", 0);
      this->m_lMYSJRecordSpecial.push_front(record);
      if ( ++nSize > 4 )
        break;
      result.nextRow();
    }
    v5 = Answer::MySqlDBGuard::query(
           &db,
           "SELECT * FROM `log_moyushijie_reward` WHERE `special`=0 ORDER BY `time` DESC LIMIT 30");
    Answer::MySqlQuery::operator=(&result, v5);
    while ( !result.eof() )
    {
      memset(&record, 0, sizeof(record));
      record.cid = Answer::MySqlQuery::getInt64Value(&result, "cid", 0);
      v6 = Answer::MySqlQuery::getStringValue(&result, "name", "");
      snprintf(record.name, 0x1Eu, v6);
      record.dungeonid = result.getIntValue("dungeonid", 0);
      record.itemid = result.getIntValue("itemid", 0);
      record.itemclass = result.getIntValue("itemclass", 0);
      record.itemcount = result.getIntValue("itemcount", 0);
      record.time = result.getIntValue("time", 0);
      record.special = result.getIntValue("special", 0);
      this->m_lMYSJRecord.push_front(record);
      if ( ++nSize > 29 )
        break;
      result.nextRow();
    }
    /* result auto-destructed */
    /* db auto-destructed */
  }
}


void GameService::SendMoYuShiJieRecord(Player *player)
{
  int8_t ConnId; 


  const LogMoYuShiJieReward *v5; 
  const LogMoYuShiJieReward *v6; 
  const LogMoYuShiJieReward *v7; 
  const LogMoYuShiJieReward *v8; 
  const LogMoYuShiJieReward *v9; 
  const LogMoYuShiJieReward *v10; 
  const LogMoYuShiJieReward *v11; 
  const LogMoYuShiJieReward *v12; 
  const LogMoYuShiJieReward *v13; 
  const LogMoYuShiJieReward *v14; 
  const LogMoYuShiJieReward *v15; 
  const LogMoYuShiJieReward *v16; 
  const LogMoYuShiJieReward *v17; 
  const LogMoYuShiJieReward *v18; 
  const LogMoYuShiJieReward *v19; 
  const LogMoYuShiJieReward *v20; 


  int8_t v23; 
  std::_List_const_iterator<LogMoYuShiJieReward> iter_0;
  std::_List_const_iterator<LogMoYuShiJieReward> iter;
  std::_List_iterator<LogMoYuShiJieReward> __x;
  std::_List_const_iterator<LogMoYuShiJieReward> v28;
  std::_List_iterator<LogMoYuShiJieReward> v29;
  std::string val;
  std::_List_iterator<LogMoYuShiJieReward> v32;
  std::_List_const_iterator<LogMoYuShiJieReward> v33;
  std::_List_iterator<LogMoYuShiJieReward> v34;
  std::string v35;
  Answer::NetPacket *packet;

  if ( player )
  {
    ConnId = Player::getConnId(player);
    packet = popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2ED7u);
    if ( packet )
    {
      Answer::MutexGuard lock(&this->m_mysjRecordLock);
      v3 = this->m_lMYSJRecord.size();
      v4 = this->m_lMYSJRecordSpecial.size();
      Answer::NetPacket::writeInt16(packet, v3 + v4);
      __x = this->m_lMYSJRecordSpecial.begin();
      iter = __x;
      while ( 1 )
      {
        v29 = this->m_lMYSJRecordSpecial.end();
        v28 = v29;
        if ( iter == v28 )
          break;
        v5 = iter.operator->();
        Answer::NetPacket::writeInt64(packet, v5->cid);
                v6 = iter.operator->();
                Answer::NetPacket::writeUTF8(packet, &val);
                        v7 = iter.operator->();
        Answer::NetPacket::writeInt32(packet, v7->dungeonid);
        v8 = iter.operator->();
        Answer::NetPacket::writeInt32(packet, v8->itemid);
        v9 = iter.operator->();
        Answer::NetPacket::writeInt8(packet, v9->itemclass);
        v10 = iter.operator->();
        Answer::NetPacket::writeInt32(packet, v10->itemcount);
        v11 = iter.operator->();
        Answer::NetPacket::writeInt32(packet, v11->time);
        v12 = iter.operator->();
        Answer::NetPacket::writeInt8(packet, v12->special);
        ++iter;
      }
      v32 = this->m_lMYSJRecord.begin();
      iter_0 = v32;
      while ( 1 )
      {
        v34 = this->m_lMYSJRecord.end();
        v33 = v34;
        if ( iter_0 == v33 )
          break;
        v13 = iter_0.operator->();
        Answer::NetPacket::writeInt64(packet, v13->cid);
                v14 = iter_0.operator->();
                Answer::NetPacket::writeUTF8(packet, &v35);
                        v15 = iter_0.operator->();
        Answer::NetPacket::writeInt32(packet, v15->dungeonid);
        v16 = iter_0.operator->();
        Answer::NetPacket::writeInt32(packet, v16->itemid);
        v17 = iter_0.operator->();
        Answer::NetPacket::writeInt8(packet, v17->itemclass);
        v18 = iter_0.operator->();
        Answer::NetPacket::writeInt32(packet, v18->itemcount);
        v19 = iter_0.operator->();
        Answer::NetPacket::writeInt32(packet, v19->time);
        v20 = iter_0.operator->();
        Answer::NetPacket::writeInt8(packet, v20->special);
        ++iter_0;
      }
      /* MutexGuard destroyed */
  uint32_t wOffset = 0;
      wOffset = packet->getWOffset();
      packet->setSize(wOffset);
      GateIndex = Player::getGateIndex(player);
      v23 = Player::getConnId(player);
      sendPacketTo(v23, GateIndex, packet);
    }
  }
}


void GameService::AddMoYuShiJieRecord(const LogMoYuShiJieReward *const logReward)
{
  GameService *v2; 
  size_t v3; 
  size_t v4; 

  v2 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v2) != 9 && logReward->special >= 0 )
  {
    if ( logReward->special )
    {
      Answer::MutexGuard lock_0(&this->m_mysjRecordLock);
      this->m_lMYSJRecordSpecial.push_front(logReward);
      if ( this->m_lMYSJRecordSpecial.size() <= 5 )
      {
        v4 = this->m_lMYSJRecordSpecial.size();
        if ( v4 + this->m_lMYSJRecord.size() > 0x1E )
          this->m_lMYSJRecord.pop_front();
      }
      else
      {
        this->m_lMYSJRecordSpecial.pop_back();
      }
      /* MutexGuard destroyed */
    }
    else
    {
      Answer::MutexGuard lock(&this->m_mysjRecordLock);
      this->m_lMYSJRecord.push_front(logReward);
      v3 = this->m_lMYSJRecord.size();
      if ( v3 + this->m_lMYSJRecordSpecial.size() > 0x1E )
        this->m_lMYSJRecord.pop_back();
      /* MutexGuard destroyed */
    }
  }
}


void GameService::ResetFestivalData(int32_t nVersion)
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    player = iter->second;
    if ( player )
      Player::ResetFestivalData(player, nVersion);
  }
  /* MutexGuard destroyed */
}


void GameService::UpdateTencentInfo(CharId_t cid, const TencentInfo *const info)
{
  CExtCharTencent *CharTencent; 
  CharId_t cida;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  cida = cid;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter = this->m_players.find(cida);
  __x = this->m_players.end();
  if ( iter != __x )
  {
    player = iter->second;
    if ( player )
    {
      CharTencent = Player::GetCharTencent(player);
      CExtCharTencent::UpdateTencentInfo(CharTencent, info);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::FamilyBroadcast( int8_t connid, FamilyId_t nFamilyId, Answer::NetPacket *inPacket)
{


  char *Buffer; 

  Answer::NetPacket *packet;

  if ( inPacket )
  {
    packet = popNetpacket(connid, Answer::PackType::PACK_DISPATCH, 0x4E4Fu);
    if ( packet )
    {
      packet->writeInt64(nFamilyId);
      Proc = inPacket->getProc();
      Answer::NetPacket::writeUInt16(packet, Proc);
      Size = inPacket->getSize();
      Buffer = inPacket->getBuffer();
      packet->write(Buffer, Size);
  uint32_t wOffset = 0;
      wOffset = packet->getWOffset();
      packet->setSize(wOffset);
      sendPacket(connid, packet);
    }
    inPacket->destroy();
  }
}


void GameService::onSocialInitFamilyInfo( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  FamilyManager *v3; 

  if ( pConn )
  {
    if ( inPacket )
    {
      v3 = Answer::Singleton<FamilyManager>::instance();
      FamilyManager::OnInitFamilyInfo(v3, inPacket);
    }
  }
}


void GameService::broadcastCityWarWinnerLogin( CharId_t nCharId, const std::string *const strName)
{
  GameService *v3; 

  GameService *v5; 
  Answer::NetPacket *packet;

  v3 = Answer::Singleton<GameService>::instance();
  packet = GameService::popNetpacket(v3, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
  if ( packet )
  {
    packet->writeInt32(334);
    packet->writeUTF8(strName);
    packet->writeInt64(nCharId);
  uint32_t wOffset = 0;
    wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    v5 = Answer::Singleton<GameService>::instance();
    GameService::worldBroadcast(v5, packet);
  }
}


void GameService::broadCastKiaFuHuoDongIcon()
{
  CKaiFuHuoDong *v1; 
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter = this->m_players.begin();
        ;
        ++iter )
  {
    __x = this->m_players.end();
    if ( iter == __x )
      break;
    player = iter->second;
    if ( player )
    {
      v1 = Answer::Singleton<CKaiFuHuoDong>::instance();
      CKaiFuHuoDong::SendKaiFuHuoDongIcon(v1, player);
    }
  }
  /* MutexGuard destroyed */
}

