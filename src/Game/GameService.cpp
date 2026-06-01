#include "GameService.h"

// External includes (available in full project)
// #include "Player.h"
// #include "CObjPet.h"
// #include "Monster.h"
// #include "Trailer.h"
// #include "Dungeon.h"
// #include "User.h"
// #include "Unit.h"
// #include "MutiConn.h"
// #include "Answer/NetPacket.h"
// #include "Answer/Mutex.h"
// #include "Answer/MutexGuard.h"
// #include "Answer/TcpClient.h"
// #include "Answer/InetAddress.h"
// #include "Answer/Thread.h"
// #include "Answer/Logger.h"
// #include "Answer/Singleton.h"
// #include "Answer/DBPool.h"
// #include "Answer/MySqlDBGuard.h"
// #include "Answer/MySqlQuery.h"
// #include "Answer/DayTime.h"
// #include "Answer/MD5.h"
// #include "Answer/Random.h"
// #include "Answer/TcpService.h"

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
  std::map<int8_t,GameService::ConnType*>::ConnType *v5; // rbx
  bool v6; // bl
  std::map<int8_t,GameService::ConnType*>::ConnType **v7; // rax

  std::string name;
  Answer::InetAddress serverAddr;
  std::string val;
  std::string v17;
  std::string v18;
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;
  Answer::NetPacket *packet;

  porta = port;
  ida[0] = id;
  if ( id < 0 || ida[0] > 99 )
    return 0;
      v5 = new ConnType(0x2000, 0x2000, 0x7F9u, 0, 0x7D0u, name, ida[0], this);
  pConn = v5;
      if ( !pConn )
    return 0;
  Answer::InetAddress::InetAddress(&serverAddr, p_host, porta);
  v6 = !Answer::TcpClient::connect(pConn, &serverAddr);
  Answer::InetAddress::~InetAddress(&serverAddr);
  if ( v6 )
    return 0;
  Answer::Thread::start(pConn);
  v7 = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::operator[](
         &this->m_mConn,
         ida);
  *v7 = pConn;
  packet = Answer::TcpClient::popNetpacket(pConn);
  if ( !packet )
    return 0;
  Answer::NetPacket::writeInt32(packet, this->m_line);
  GameService::GetVersion((GameService *const)&val);
  Answer::NetPacket::writeUTF8(packet, &val);
    GetServerVersion();
  Answer::NetPacket::writeUTF8(packet, &v17);
    Answer::TcpClient::getName((Answer::TcpClient *const)&v18);
  Answer::NetPacket::writeUTF8(packet, &v18);
    WOffset = Answer::NetPacket::getWOffset(packet);
  Answer::NetPacket::setSize(packet, WOffset);
  Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
  Answer::NetPacket::setProc(packet, 0x4E22u);
  Answer::TcpClient::sendPacket(pConn, packet);
  return 1;
}


std::string GameService::GetVersion()
{
  Answer::Inifile ini;
  std::string inifile;
  std::string key;
        Answer::Inifile::Inifile(&ini);
  Answer::Inifile::parse(&ini, &inifile);
          Answer::Inifile::getStrValue((Answer::Inifile *const)this, (const std::string *const)&ini, &key);
          Answer::Inifile::~Inifile(&ini);
    return (std::string)this;
}


void GameService::RecalAllPlayerAttr()
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter, 0) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    pPlayer = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( pPlayer )
    {
      Player::RecalcAttr(pPlayer);
      break;
    }
  }
  /* MutexGuard destroyed */
}


void GameService::AddPlayerVipClubDropTime()
{
  CVip *PlayerVip; // rax
  Answer::DBPool *v2; // rax
  Answer::MySqlDBGuard db;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter, 0) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    pPlayer = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( pPlayer )
    {
      PlayerVip = Player::GetPlayerVip(pPlayer);
      CVip::AddClubDropTime(PlayerVip);
    }
  }
  v2 = Answer::Singleton<Answer::DBPool>::instance();
  Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
  Answer::MySqlDBGuard::excute(&db, "update mem_chr_vip set drop_time = 1 where club > 0 and drop_time <= 0");
  Answer::MySqlDBGuard::~MySqlDBGuard(&db);
  /* MutexGuard destroyed */
}


void GameService::onNetPacket( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t Id; // bl
  CZongHeYunYingHD *v4; // rax
  if ( pConn && inPacket )
  {
    proc = Answer::NetPacket::getProc(inPacket);
    connid = MutiConn<GameService>::GetId(pConn);
    if ( proc <= 0x4E20u || proc > 0x61AAu )
    {
      cgindex = Answer::NetPacket::readInt32(inPacket);
      if ( cgindex > 0 && cgindex <= 9999 )
      {
        if ( proc == 6 )
        {
          GameService::onEnterGame(this, connid, cgindex, inPacket);
        }
        else if ( proc == 7 )
        {
          GameService::onEnterGameRobot(this, connid, cgindex, inPacket);
        }
        else
        {
          GameService::onGameNetpacket(this, connid, cgindex, inPacket);
        }
      }
    }
    else if ( proc <= 0x4E54u || proc > 0x4E77u )
    {
      switch ( proc )
      {
        case 0x4E26u:
          GameService::onAddUser(this, pConn, inPacket);
          break;
        case 0x4E27u:
          GameService::onRemoveUser(this, pConn, inPacket);
          break;
        case 0x4E29u:
          GameService::onGMReloadItem(this, pConn, inPacket);
          break;
        case 0x4E2Du:
          GameService::onGateDisconnect(this, pConn, inPacket);
          break;
        case 0x4E7Eu:
          GameService::onSocialUpdatePetRank(this, pConn, inPacket);
          break;
        case 0x4E7Fu:
          GameService::onSocialUpdateFamilyInfo(this, pConn, inPacket);
          break;
        case 0x4E80u:
          GameService::onSocialUpdateRankMirror(this, pConn, inPacket);
          break;
        case 0x4E81u:
          GameService::onSetWorldLevel(this, pConn, inPacket);
          break;
        case 0x4E82u:
          GameService::OnReceiveDaTiResult(this, pConn, inPacket);
          break;
        case 0x4E83u:
          GameService::OnSocialCityMasterBanChat(this, pConn, inPacket);
          break;
        case 0x4E86u:
          GameService::onSocialInitPetRank(this, pConn, inPacket);
          break;
        case 0x4E87u:
          GameService::onSocialCreateTeamDungeon(this, pConn, inPacket);
          break;
        case 0x4E88u:
          GameService::OnReceiveDaTiRank(this, pConn, inPacket);
          break;
        case 0x4E89u:
          GameService::onSocialSendSystemMail(this, pConn, inPacket);
          break;
        case 0x4E8Au:
          GameService::onSocialSendSystemMail2(this, pConn, inPacket);
          break;
        case 0x4E8Bu:
          GameService::onSocialInitRankMirror(this, pConn, inPacket);
          break;
        case 0x4E8Cu:
          GameService::onSocialQueryPlayerInfo(this, pConn, inPacket);
          break;
        case 0x4E8Du:
          GameService::onRequestDropRecord(this, pConn, inPacket);
          break;
        case 0x4E8Eu:
          GameService::onSocialInitFamilyInfo(this, pConn, inPacket);
          break;
        case 0x4EC1u:
          Id = MutiConn<GameService>::GetId(pConn);
          v4 = Answer::Singleton<CZongHeYunYingHD>::instance();
          CZongHeYunYingHD::UpdatePlayerInfo(v4, Id, inPacket);
          break;
        case 0x4EC4u:
          GameService::onUpdateActivityState(this, pConn, inPacket);
          break;
        case 0x4EEAu:
          GameService::qqToPlayer(this, pConn, inPacket);
          break;
        default:
          break;
      }
    }
    else
    {
      GameService::onSocialNetpacket(this, pConn, inPacket);
    }
    Answer::NetPacket::destroy(inPacket);
  }
}


void GameService::startGame()
{
  TileManager *v1; // rax
  MapManager *v2; // rax
  MapManager *v3; // rax

  CActivityManager *v5; // rax

  CGMBackstate *v7; // rax

  CFestivalDoubleEleven *v9; // rax

  CZongHeYunYingHD *v11; // rax

  CKiaFuRecharge *v13; // rax
  CFestivalActivity *v14; // rax
  GameService *v15; // rax
  CfgData *v16; // rax


  GameService *v19; // rax
  Answer::NetPacket *packet;

  Player::initNetPacketHandlers();
  GameService::InitServerBattle(this);
  v1 = Answer::Singleton<TileManager>::instance();
  TileManager::Init(v1);
  v2 = Answer::Singleton<MapManager>::instance();
  MapManager::Init(v2);
  v3 = Answer::Singleton<MapManager>::instance();
  MapManager::StartAll(v3);
  m_line = this->m_line;
  v5 = Answer::Singleton<CActivityManager>::instance();
  CActivityManager::Init(v5, m_line);
  v6 = this->m_line;
  v7 = Answer::Singleton<CGMBackstate>::instance();
  CGMBackstate::Init(v7, v6);
  v8 = this->m_line;
  v9 = Answer::Singleton<CFestivalDoubleEleven>::instance();
  CFestivalDoubleEleven::Init(v9, v8);
  v10 = this->m_line;
  v11 = Answer::Singleton<CZongHeYunYingHD>::instance();
  CZongHeYunYingHD::Init(v11, v10);
  v12 = this->m_line;
  v13 = Answer::Singleton<CKiaFuRecharge>::instance();
  CKiaFuRecharge::Init(v13, v12);
  v14 = Answer::Singleton<CFestivalActivity>::instance();
  CFestivalActivity::Init(v14);
  GameService::InitDropTimes(this);
  GameService::InitMoYuShiJieRecord(this);
  GameService::requestSocialData(this);
  GameService::requestWorldLevel(this);
  if ( this->m_line == 1 )
  {
    GameService::SendServerDiffToGlobal(this);
    v15 = Answer::Singleton<GameService>::instance();
    packet = GameService::popNetpacket(v15, Answer::PackType::PACK_PROC, 0x4E30u);
    if ( packet )
    {
      v16 = Answer::Singleton<CfgData>::instance();
      Debug = CfgData::getDebug(v16);
      Answer::NetPacket::writeInt32(packet, Debug);
      WOffset = Answer::NetPacket::getWOffset(packet);
      Answer::NetPacket::setSize(packet, WOffset);
      v19 = Answer::Singleton<GameService>::instance();
      GameService::sendPacket(v19, 0, packet);
    }
  }
}


void GameService::stopGame()
{
  MapManager *v1; // rax
  GameService *v2; // rax

  v1 = Answer::Singleton<MapManager>::instance();
  MapManager::StopAll(v1);
  v2 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v2) != 9 )
    GameService::saveAllPlayerToDB(this);
}


int8_t GameService::GetAutoConnId()
{
  const MutiConn<GameService> *pConn;

  pConn = GameService::GetConn(this);
  if ( pConn )
    return MutiConn<GameService>::GetId(pConn);
  else
    return 0;
}


int8_t GameService::GetConnId(CharId_t cid)
{
  int8_t ConnId; // bl
  CharId_t cida;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  cida = cid;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter._M_node = std::map<long,Player *>::find(&this->m_players, &cida)._M_node;
  __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
  if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x)
    && (pPlayer = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second) != 0 )
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
  DBService *v1; // rax

  if ( this->m_line == 1 )
  {
    v1 = Answer::Singleton<DBService>::instance();
    DBService::OnUpdatePay(v1);
  }
}


void GameService::onNewMinuteCome(int32_t minute)
{
  DBService *v2; // rax

  std::pair<const int,ChatValidate> *v4; // rax
  std::pair<const int,ChatValidate> *v5; // rax
  std::_Rb_tree_iterator<std::pair<const int,ChatValidate> > v6; // rax
  std::_Rb_tree_iterator<std::pair<const int,ChatValidate> > it;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const int,ChatValidate> > __x;
  Answer::NetPacket *packet;
  if ( this->m_line == 1 )
  {
    v2 = Answer::Singleton<DBService>::instance();
    DBService::onNewMinuteCome(v2, minute);
    packet = GameService::popNetpacket(this, 0, Answer::PackType::PACK_PROC, 0x4E38u);
    if ( !packet )
      return;
    Answer::NetPacket::writeInt32(packet, minute);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    sendPacket(0, packet);
  }
  if ( !(minute % 5) )
  {
    Time = Answer::DayTime::now();
    Answer::MutexGuard lock(&this->m_ChatValidateLock);
    it._M_node = std::map<int,ChatValidate>::begin(&this->m_ChatValidateMap)._M_node;
    while ( 1 )
    {
      __x._M_node = std::map<int,ChatValidate>::end(&this->m_ChatValidateMap)._M_node;
      if ( !std::_Rb_tree_iterator<std::pair<int const,ChatValidate>>::operator!=(&it, &__x) )
        break;
      v4 = std::_Rb_tree_iterator<std::pair<int const,ChatValidate>>::operator->(&it);
      if ( Time - v4->second.Time <= 300 )
      {
        std::_Rb_tree_iterator<std::pair<int const,ChatValidate>>::operator++(&it, 0);
      }
      else
      {
        if ( std::_Rb_tree_iterator<std::pair<int const,ChatValidate>>::operator->(&it)->second.SendPacket )
        {
          v5 = std::_Rb_tree_iterator<std::pair<int const,ChatValidate>>::operator->(&it);
          Answer::NetPacket::destroy(v5->second.SendPacket);
        }
        v6._M_node = std::_Rb_tree_iterator<std::pair<int const,ChatValidate>>::operator++(&it, 0)._M_node;
        std::map<int,ChatValidate>::erase(&this->m_ChatValidateMap, v6);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::OnDaySwitch()
{
  if ( this->m_line == 1 )
  {
    GameService::ResetDropTimes(this);
    GameService::SendServerDiffToGlobal(this);
  }
}


void GameService::onPlayerLoaded( PlayerDBData *const dbData, int32_t reason, bool isRobot)
{

  MapManager *v5; // rax
  CfgData *v7; // rax

  MapManager *v9; // rax
  PlayerRobot *v10; // rbx
  CPoolManager *v11; // rax


  MapManager *v15; // rax
  CExtOperateLimit *OperateLimit; // rax

  CActivityManager *v18; // rax
  GameService *v19; // rax

  Answer::MutexGuard lock;
  TencentInfo info;
  Player *player;
  Map *pMap;
  User *pUser;
  Map *pMap_0;
  CfgMapRegion *pCfgRegion;
  Map *pTempMap;
  Answer::NetPacket *packet;

  player = GameService::getPlayer(this, dbData->chr.data.cid, 0, 0);
  if ( player )
  {
    pMap_0 = StaticObj::getMap(player);
    mapid = 0;
    if ( pMap_0 )
      mapid = Map::GetMapId(pMap_0);
    Answer::Logger::print(
      Answer::LogLevel::LOG_LEVEL_INFO,
      "GameService::onPlayerLoaded player cid = %lld, map = %p, mapid = %d already in game\n",
      dbData->chr.data.cid,
      pMap_0,
      mapid);
  }
  else
  {
    v4 = dbData->chr.data.mapid;
    v5 = Answer::Singleton<MapManager>::instance();
    pMap = MapManager::GetMap(v5, v4);
    if ( !pMap )
    {
      nRegion = 20001;
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
      pCfgRegion = CfgData::getMapRegion(v7, nRegion);
      if ( pCfgRegion )
      {
        v8 = pCfgRegion->mapid;
        v9 = Answer::Singleton<MapManager>::instance();
        pTempMap = MapManager::GetMap(v9, v8);
        if ( pTempMap )
        {
          pos = Map::getRandomWalkablePositionInRegion(pTempMap, pCfgRegion);
          if ( pos.x >= 0 && pos.y >= 0 )
          {
            pMap = pTempMap;
            dbData->chr.data.mapid = Map::GetMapId(pTempMap);
            *(Position *)&dbData->chr.data.x = pos;
          }
        }
      }
    }
    if ( pMap )
    {
      Answer::MutexGuard lock(&this->m_userLock);
      nIndex = getUserIndex(dbData->connid, dbData->cgindex);
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
            player = CPoolManager::pop<Player>(v11);
          }
          if ( player )
          {
            User::GetTencentInfo(&info, pUser);
            Player::InitTencentInfo(player, &info);
            TencentInfo::~TencentInfo(&info);
            Player::init(player, dbData);
            User::setPlayer(pUser, player);
            GameService::AddPlayer(this, player, reason);
            if ( !Player::reconnect(player) )
            {
              iparam2 = dbData->chr.data.y;
              x = dbData->chr.data.x;
              RunnerId = Map::GetRunnerId(pMap);
              v15 = Answer::Singleton<MapManager>::instance();
              MapManager::PostMsg(v15, RunnerId, GameMsgCode::GMC_PLAYER_ENTER_MAP, player, pMap, x, iparam2, 0);
              if ( GameService::getLine(this) == 9 )
              {
                OperateLimit = Player::GetOperateLimit(player);
                LimitCount = CExtOperateLimit::GetLimitCount(OperateLimit, 1057);
                v18 = Answer::Singleton<CActivityManager>::instance();
                nActId = CActivityManager::GetCurActivityId(v18, LimitCount);
                LOBYTE(LimitCount) = Player::getConnId(player);
                v19 = Answer::Singleton<GameService>::instance();
                packet = GameService::popNetpacket(v19, LimitCount, Answer::PackType::PACK_DISPATCH, 0x1Du);
                if ( packet )
                {
                  Answer::NetPacket::writeInt32(packet, nActId);
                  WOffset = Answer::NetPacket::getWOffset(packet);
                  Answer::NetPacket::setSize(packet, WOffset);
                  Player::addNetPacket(player, packet, 0);
                  Answer::NetPacket::destroy(packet);
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
  CPoolManager *v2; // rax
  Answer::MutexGuard lock;
  User *user;

  if ( player )
  {
    connid = Player::getConnId(player);
    cgindex = Player::getGateIndex(player);
    Answer::Logger::print(
      Answer::LogLevel::LOG_LEVEL_DEBUG,
      "GameService::onPlayerLogout push player %p, cgindex = %d\n",
      player,
      cgindex);
    GameService::removePlayer(this, player);
    if ( Player::getGateIndex(player) > 0 )
    {
      Player::setGateIndex(player, -1);
      if ( (*((unsigned __int8 (__fastcall **)(Player *))player->_vptr_Entity + 34))(player) )
      {
        (*((void (__fastcall **)(Player *))player->_vptr_Entity + 1))(player);
      }
      else
      {
        v2 = Answer::Singleton<CPoolManager>::instance();
        CPoolManager::push<Player>(v2, player);
      }
    }
    if ( (uint8_t)connid < 0x64u && (uint16_t)cgindex < 0x2710u )
    {
      Answer::MutexGuard lock(&this->m_userLock);
      nIndex = getUserIndex(connid, cgindex);
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

  packet = GameService::popNetpacket(this, connid, Answer::PackType::PACK_PROC, 0x4E25u);
  if ( packet )
  {
    Answer::NetPacket::writeInt64(packet, uid);
    Answer::NetPacket::writeInt32(packet, sid);
    Answer::NetPacket::writeInt32(packet, reason);
    Answer::NetPacket::writeInt64(packet, cid);
    Answer::NetPacket::writeInt32(packet, param);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    sendPacket(connid, packet);
  }
}


void GameService::sendPacket(int8_t connid, Answer::NetPacket *packet)
{
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;

  pConn = GameService::GetConn(this, connid);
  if ( pConn )
    Answer::TcpClient::sendPacket(pConn, packet);
}


void GameService::sendPacketTo( int8_t connid, int16_t index, Answer::NetPacket *inPacket)
{
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;

  pConn = GameService::GetConn(this, connid);
  if ( pConn )
    Answer::TcpService::sendPacketTo(pConn, index, inPacket);
}


int32_t GameService::replySuccess( int8_t connid, int16_t index, uint16_t proc, int64_t addon)
{
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;

  pConn = GameService::GetConn(this, connid);
  if ( pConn )
    return Answer::TcpService::replySuccess(pConn, index, proc, addon);
  else
    return 10002;
}


int32_t GameService::replyfailure( int8_t connid, int16_t index, uint16_t proc, int32_t errcode, int64_t addon)
{
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;

  pConn = GameService::GetConn(this, connid);
  if ( pConn )
    return Answer::TcpService::replyfailure(pConn, index, proc, errcode, addon);
  else
    return 10002;
}


void GameService::broadcastHuoDongDaTingIcon()
{
  std::pair<const long int,Player*> *v1; // rax
  DailyActivity *PlayerDailyActivity; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  if ( !std::map<long,Player *>::empty(&this->m_players) )
  {
    for ( it._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
          ;
          std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&it) )
    {
      __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
      if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
        break;
      v1 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
      PlayerDailyActivity = Player::GetPlayerDailyActivity(v1->second);
      DailyActivity::SendHuoDongDaTingIcon(PlayerDailyActivity);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::SendOpenBetaIcon()
{
  Player *second; // rbx
  COpenBeta *v2; // rax
  std::pair<const long int,Player*> *v3; // rax
  ShowIcon stu;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  if ( !std::map<long,Player *>::empty(&this->m_players) )
  {
    for ( it._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
          ;
          std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&it) )
    {
      __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
      if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
        break;
      second = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it)->second;
      v2 = Answer::Singleton<COpenBeta>::instance();
      COpenBeta::getIconState(&stu, v2, second);
      v3 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
      Player::SendIconState(v3->second, &stu);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::broadFamilyWarIcon()
{
  std::pair<const long int,Player*> *v1; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  if ( !std::map<long,Player *>::empty(&this->m_players) )
  {
    for ( it._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
          ;
          std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&it) )
    {
      __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
      if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
        break;
      v1 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
      Player::SendFamilyWarIcon(v1->second);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::broadcast( Answer::NetPacket *inPacket, const IndexMap *const indexMap)
{
  std::list<short int> *p_second; // r12
  char first; // dl
  std::_Rb_tree_const_iterator<std::pair<const int8_t,std::list<short int> > > iter;
  std::_Rb_tree_const_iterator<std::pair<const int8_t,std::list<short int> > > __x;

  if ( inPacket )
  {
    if ( std::map<int8_t,std::list<short>>::empty(indexMap) )
    {
      Answer::NetPacket::destroy(inPacket);
    }
    else
    {
      for ( iter._M_node = std::map<int8_t,std::list<short>>::begin(indexMap)._M_node;
            ;
            std::_Rb_tree_const_iterator<std::pair<int8_t const,std::list<short>>>::operator++(&iter) )
      {
        __x._M_node = std::map<int8_t,std::list<short>>::end(indexMap)._M_node;
        if ( !std::_Rb_tree_const_iterator<std::pair<int8_t const,std::list<short>>>::operator!=(&iter, &__x) )
          break;
        p_second = &std::_Rb_tree_const_iterator<std::pair<int8_t const,std::list<short>>>::operator->(&iter)->second;
        first = std::_Rb_tree_const_iterator<std::pair<int8_t const,std::list<short>>>::operator->(&iter)->first;
        GameService::broadcast(this, inPacket, first, p_second);
      }
      Answer::NetPacket::destroy(inPacket);
    }
  }
}


void GameService::broadcast( Answer::NetPacket *inPacket, int8_t connid, const Int16List *const indexList)
{


  int16_t *v7; // rax

  char *Buffer; // rax

  std::_List_const_iterator<short int> iter;
  std::_List_const_iterator<short int> __x;
  Answer::NetPacket *packet;

  if ( inPacket )
  {
    if ( std::list<short>::empty(indexList) )
    {
      Answer::NetPacket::destroy(inPacket);
    }
    else
    {
      nsize = std::list<short>::size(indexList);
      v4 = 2 * (nsize + 1) + Answer::NetPacket::getSize(inPacket);
      Proc = Answer::NetPacket::getProc(inPacket);
      Type = Answer::NetPacket::getType(inPacket);
      packet = GameService::popNetpacket(this, connid, Type, Proc, v4);
      if ( packet )
      {
        Answer::NetPacket::writeInt16(packet, nsize);
        for ( iter._M_node = std::list<short>::begin(indexList)._M_node;
              ;
              std::_List_const_iterator<short>::operator++(&iter) )
        {
          __x._M_node = std::list<short>::end(indexList)._M_node;
          if ( !std::_List_const_iterator<short>::operator!=(&iter, &__x) )
            break;
          v7 = (int16_t *)std::_List_const_iterator<short>::operator*(&iter);
          Answer::NetPacket::writeInt16(packet, *v7);
        }
        Size = Answer::NetPacket::getSize(inPacket);
        Buffer = Answer::NetPacket::getBuffer(inPacket);
        Answer::NetPacket::write(packet, Buffer, Size);
        WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        sendPacket(connid, packet);
      }
    }
  }
}


void GameService::broadcast(Answer::NetPacket *inPacket)
{
  std::list<short int> *v2; // rax
  IndexMap indexMap;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __it;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > v7;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v8;
  int16_t __x;
  char __k;
  Player *player;

  if ( inPacket )
  {
    std::map<int8_t,std::list<short>>::map(&indexMap);
    Answer::MutexGuard lock(&this->m_playerLock);
    __it._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
    std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::_Rb_tree_const_iterator(&iter, &__it);
    while ( 1 )
    {
      v8._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
      std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::_Rb_tree_const_iterator(&v7, &v8);
      if ( !std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::operator!=(&iter, &v7) )
        break;
      player = std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
      if ( player )
      {
        __x = Player::getGateIndex(player);
        __k = Player::getConnId(player);
        v2 = std::map<int8_t,std::list<short>>::operator[](&indexMap, &__k);
        std::list<short>::push_back(v2, &__x);
      }
      std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::operator++(&iter);
    }
    /* MutexGuard destroyed */
    GameService::broadcast(this, inPacket, &indexMap);
    std::map<int8_t,std::list<short>>::~map(&indexMap);
  }
}


void GameService::broadcast(Answer::NetPacket *inPacket, const CharIdList *const cids)
{
  const int64_t *v3; // rax
  std::list<short int> *v4; // rax
  IndexMap indexMap;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > findIter;
  Answer::MutexGuard lock;
  std::_List_const_iterator<long int> iter;
  std::_List_const_iterator<long int> v10;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  int16_t GateIndex;
  char __k;
  Player *player;

  if ( inPacket )
  {
    if ( std::list<long>::empty(cids) )
    {
      Answer::NetPacket::destroy(inPacket);
    }
    else
    {
      std::map<int8_t,std::list<short>>::map(&indexMap);
      for ( iter._M_node = std::list<long>::begin(cids)._M_node; ; std::_List_const_iterator<long>::operator++(&iter) )
      {
        v10._M_node = std::list<long>::end(cids)._M_node;
        if ( !std::_List_const_iterator<long>::operator!=(&iter, &v10) )
          break;
        Answer::MutexGuard lock(&this->m_playerLock);
        v3 = std::_List_const_iterator<long>::operator*(&iter);
        findIter._M_node = std::map<long,Player *>::find(&this->m_players, v3)._M_node;
        __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
        if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&findIter, &__x) )
        {
          player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&findIter)->second;
          if ( player )
          {
            GateIndex = Player::getGateIndex(player);
            __k = Player::getConnId(player);
            v4 = std::map<int8_t,std::list<short>>::operator[](&indexMap, &__k);
            std::list<short>::push_back(v4, &GateIndex);
          }
        }
        /* MutexGuard destroyed */
      }
      GameService::broadcast(this, inPacket, &indexMap);
      std::map<int8_t,std::list<short>>::~map(&indexMap);
    }
  }
}


void GameService::broadcastToFamily( Answer::NetPacket *inPacket, FamilyId_t nFamilyId, int32_t nLevel)
{
  std::list<short int> *v5; // rax
  IndexMap indexMap;
  std::_Rb_tree_const_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
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
      std::map<int8_t,std::list<short>>::map(&indexMap);
      Answer::MutexGuard lock(&this->m_playerLock);
      __it._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
      std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::_Rb_tree_const_iterator(&iter, &__it);
      while ( 1 )
      {
        v13._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
        std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::_Rb_tree_const_iterator(&v12, &v13);
        if ( !std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::operator!=(&iter, &v12) )
          break;
        player = std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
        if ( player
          && Player::getFamilyId(player) == nFamilyId
          && (*((int (__fastcall **)(Player *))player->_vptr_Entity + 9))(player) > nLevel )
        {
          __x = Player::getGateIndex(player);
          __k = Player::getConnId(player);
          v5 = std::map<int8_t,std::list<short>>::operator[](&indexMap, &__k);
          std::list<short>::push_back(v5, &__x);
        }
        std::_Rb_tree_const_iterator<std::pair<long const,Player *>>::operator++(&iter);
      }
      /* MutexGuard destroyed */
      GameService::broadcast(this, inPacket, &indexMap);
      std::map<int8_t,std::list<short>>::~map(&indexMap);
    }
    else
    {
      Answer::NetPacket::destroy(inPacket);
    }
  }
}


void GameService::worldBroadcast(int8_t connid, Answer::NetPacket *inPacket)
{


  char *Buffer; // rcx

  Answer::NetPacket *packet;

  if ( inPacket )
  {
    oldSize = Answer::NetPacket::getSize(inPacket);
    if ( Answer::NetPacket::rightShift(inPacket, 2u) )
    {
      Answer::NetPacket::writeInt16(inPacket, -1);
      Answer::NetPacket::setSize(inPacket, oldSize + 2);
      sendPacket(connid, inPacket);
    }
    else
    {
      v3 = Answer::NetPacket::getSize(inPacket) + 2;
      Proc = Answer::NetPacket::getProc(inPacket);
      Type = Answer::NetPacket::getType(inPacket);
      packet = GameService::popNetpacket(this, connid, Type, Proc, v3);
      if ( packet )
      {
        Answer::NetPacket::writeInt16(packet, -1);
        Size = Answer::NetPacket::getSize(inPacket);
        Buffer = Answer::NetPacket::getBuffer(inPacket);
        Answer::NetPacket::write(packet, Buffer, Size);
        WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        sendPacket(connid, packet);
        Answer::NetPacket::destroy(inPacket);
      }
    }
  }
}


void GameService::worldBroadcast(Answer::NetPacket *inPacket)
{


  char *Buffer; // rax


  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > iter;
  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > __x;
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;
  Answer::NetPacket *packet;

  if ( inPacket )
  {
    for ( iter._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::begin(&this->m_mConn)._M_node;
          ;
          std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator++(&iter) )
    {
      __x._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::end(&this->m_mConn)._M_node;
      if ( !std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator!=(&iter, &__x) )
        break;
      pConn = std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator->(&iter)->second;
      if ( pConn )
      {
        Size = Answer::NetPacket::getSize(inPacket);
        packet = Answer::TcpClient::popNetpacket(pConn, Size + 2);
        if ( packet )
        {
          Answer::NetPacket::writeInt16(packet, -1);
          v3 = Answer::NetPacket::getSize(inPacket);
          Buffer = Answer::NetPacket::getBuffer(inPacket);
          Answer::NetPacket::write(packet, Buffer, v3);
          WOffset = Answer::NetPacket::getWOffset(packet);
          Answer::NetPacket::setSize(packet, WOffset);
          Type = Answer::NetPacket::getType(inPacket);
          Answer::NetPacket::setType(packet, Type);
          Proc = Answer::NetPacket::getProc(inPacket);
          Answer::NetPacket::setProc(packet, Proc);
          Answer::TcpClient::sendPacket(pConn, packet);
        }
      }
    }
    Answer::NetPacket::destroy(inPacket);
  }
}


void GameService::broadcastActivityState(int32_t nId, int8_t nState)
{

  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > iter;
  std::_Rb_tree_iterator<std::pair<const int8_t,GameService::ConnType*> > __x;
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;
  Answer::NetPacket *packet;

  for ( iter._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::begin(&this->m_mConn)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator++(&iter) )
  {
    __x._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::end(&this->m_mConn)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator!=(&iter, &__x) )
      break;
    pConn = std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
      packet = Answer::TcpClient::popNetpacket(pConn);
      if ( packet )
      {
        Answer::NetPacket::writeInt32(packet, nId);
        Answer::NetPacket::writeInt8(packet, nState);
        WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_DISPATCH);
        Answer::NetPacket::setProc(packet, 0x4EC4u);
        Answer::TcpClient::sendPacket(pConn, packet);
      }
    }
  }
}


void GameService::AddPlayer(Player *player, int32_t reason)
{
  FamilyId_t FamilyId; // rbx

  CActivityManager *v5; // rax
  CharId_t Cid; // rcx


  int8_t v10; // bl
  DBService *v11; // rax
  Answer::MutexGuard lock;
  int64_t __k;
  std::string strName;

  if ( player )
  {
    Answer::MutexGuard lock(&this->m_playerLock);
    __k = Player::getCid(player);
    *std::map<long,Player *>::operator[](&this->m_players, &__k) = player;
    /* MutexGuard destroyed */
    if ( reason != 3 )
    {
      FamilyId = Player::getFamilyId(player);
      ConnId = Player::getConnId(player);
      v5 = Answer::Singleton<CActivityManager>::instance();
      if ( FamilyId == CActivityManager::GetCityWarWinner(v5, ConnId) && Player::getFamilyPosition(player) == 3 )
      {
        Player::getName((const Player *const)&strName);
        Cid = Player::getCid(player);
        GameService::broadcastCityWarWinnerLogin(this, Cid, &strName);
              }
    }
    GameService::sendSocialAddPlayer(this, player);
    Player::sendBasicInfo(player, reason);
    LastLoginTime = Player::getLastLoginTime(player);
    v9 = Player::getCid(player);
    v10 = Player::getConnId(player);
    v11 = Answer::Singleton<DBService>::instance();
    DBService::UpdateLoginTime(v11, v10, v9, LastLoginTime);
  }
}


void GameService::removePlayer(Player *player)
{
  CharId_t Cid; // rax
  std::pair<const long int,Player*> *v3; // rax
  int64_t v4; // rbx
  std::pair<const long int,Player*> *v5; // rax

  std::pair<const long int,Player*> *v7; // rax

  CharId_t v9; // rbx
  int64_t v10; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  int64_t __x;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v14;

  if ( player )
  {
    Answer::MutexGuard lock(&this->m_playerLock);
    __x = Player::getCid(player);
    it._M_node = std::map<long,Player *>::find(&this->m_players, &__x)._M_node;
    v14._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator==(&it, &v14) )
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
      v3 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
      v4 = (*((int64_t (__fastcall **)(Player *))v3->second->_vptr_Entity + 2))(v3->second);
      if ( v4 == (*((int64_t (__fastcall **)(Player *))player->_vptr_Entity + 2))(player) )
      {
        std::map<long,Player *>::erase(&this->m_players, it);
        GameService::sendSocialRemovePlayer(this, player);
      }
      else
      {
        v5 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
        v6 = Player::getCid(v5->second);
        v7 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
        v8 = (*((int64_t (__fastcall **)(Player *))v7->second->_vptr_Entity + 2))(v7->second);
        v9 = Player::getCid(player);
        v10 = (*((int64_t (__fastcall **)(Player *))player->_vptr_Entity + 2))(player);
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
{
  bool v4; // bl
  int8_t ConnId; // bl


  CharId_t v8; // rax

  char *Buffer; // rax


  ChatValidate *v13; // rbx
  ChatValidate *v14; // rbx
  ChatValidate *v15; // rbx
  std::string Key;
  std::string Md5String;
  std::string Cid;
  std::string Sid;
  std::string Name;
  std::string PassPort;
  Answer::MutexGuard lockChat;
  Answer::MutexGuard lock;
  std::string v32;
  std::string __rhs;
  Answer::MD5 v35;
  Answer::NetPacket *packet;
  if ( !inPacket || !Owner )
    return 0;
  Answer::MutexGuard lock(&this->m_playerLock);
  Answer::MutexGuard lockChat(&this->m_ChatValidateLock);
  ConnId = Player::getConnId(Owner);
  packet = GameService::popNetpacket(this, ConnId, Answer::PackType::PACK_DISPATCH, 0x2ED8u);
  if ( packet )
  {
    Player::GetPassport((Player *const)&PassPort);
    Player::getName((const Player *const)&Name);
    SidInt = Player::getSid(Owner);
                v6 = std::ios::out | std::ios::in;
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::basic_stringstream(v19, (uint32_t)v6);
    std::ostream::operator<<(v20, (uint32_t)SidInt);
    std::operator>><char>(v19, &Sid);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::~basic_stringstream(v19);
                v7 = std::ios::out | std::ios::in;
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::basic_stringstream(v19, (uint32_t)v7);
    v8 = Player::getCid(Owner);
    std::ostream::operator<<(v20, v8);
    std::operator>><char>(v19, &Cid);
    std::basic_stringstream<char,std::char_traits<char>,std::allocator<char>>::~basic_stringstream(v19);
    Player::GetPlatform((const Player *const)v31);
    std::operator+<char>((const char *)__lhs, &stru_8DF6CD);
    std::operator+<char>(&__rhs, __lhs);
    std::operator+<char>(&v32, &__rhs);
    std::operator+<char>(&Md5String, &v32);
                    Answer::MD5::MD5((Answer::MD5 *const)v35.count);
    Answer::MD5::md5sum(&v35, (const std::string *const)v35.count);
    std::operator+<char>(&Key, (const std::string *const)&v35);
        Answer::MD5::~MD5((Answer::MD5 *const)v35.count);
    ++*/* member access via offset */;
    Answer::NetPacket::writeInt8(packet, 1);
    Answer::NetPacket::writeInt32(packet, */* member access via offset */);
    Answer::NetPacket::writeUTF8(packet, &Key);
    Size = Answer::NetPacket::getSize(inPacket);
    Buffer = Answer::NetPacket::getBuffer(inPacket);
    Answer::NetPacket::write(packet, Buffer, Size);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    GateIndex = Player::getGateIndex(Owner);
    LOBYTE(Size) = Player::getConnId(Owner);
    GameService::sendPacketTo(this, Size, GateIndex, packet);
    std::map<int,ChatValidate>::operator[](
      &this->m_ChatValidateMap,
      (const int *const)((char *)&loc_7A1530 + (int64_t)this))->SendPacket = inPacket;
    v13 = std::map<int,ChatValidate>::operator[](
            &this->m_ChatValidateMap,
            (const int *const)((char *)&loc_7A1530 + (int64_t)this));
    v13->CharId = Player::getCid(Owner);
    v14 = std::map<int,ChatValidate>::operator[](
            &this->m_ChatValidateMap,
            (const int *const)((char *)&loc_7A1530 + (int64_t)this));
    v14->Time = Unit::getNow(Owner);
    std::map<int,ChatValidate>::operator[](
      &this->m_ChatValidateMap,
      (const int *const)((char *)&loc_7A1530 + (int64_t)this))->Channel = Channel;
    v15 = std::map<int,ChatValidate>::operator[](
            &this->m_ChatValidateMap,
            (const int *const)((char *)&loc_7A1530 + (int64_t)this));
    v15->SendCharId = Player::getCid(Owner);
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
  Answer::MutexGuard lock;
  if ( pPet )
  {
    Answer::MutexGuard lock(&this->m_petsLock);
    __k[0] = CObjPet::GetPetId(pPet);
    *std::map<long,CObjPet *>::operator[](&this->m_pets, __k) = pPet;
    /* MutexGuard destroyed */
  }
}


void GameService::removePet(CObjPet *pPet)
{
  Answer::MutexGuard lock;
  if ( pPet )
  {
    Answer::MutexGuard lock(&this->m_petsLock);
    __x[0] = CObjPet::GetPetId(pPet);
    std::map<long,CObjPet *>::erase(&this->m_pets, __x);
    /* MutexGuard destroyed */
  }
}


void GameService::addMonster(Monster *monster)
{
  Answer::MutexGuard lock;
  if ( monster )
  {
    Answer::MutexGuard lock(&this->m_monstersLock);
    __k[0] = (*((int64_t (__fastcall **)(Monster *))monster->_vptr_Entity + 2))(monster);
    *std::map<long,Monster *>::operator[](&this->m_monsters, __k) = monster;
    /* MutexGuard destroyed */
  }
}


void GameService::removeMonster(Monster *monster)
{
  Answer::MutexGuard lock;
  if ( monster )
  {
    Answer::MutexGuard lock(&this->m_monstersLock);
    __x[0] = (*((int64_t (__fastcall **)(Monster *))monster->_vptr_Entity + 2))(monster);
    std::map<long,Monster *>::erase(&this->m_monsters, __x);
    /* MutexGuard destroyed */
  }
}


void GameService::addTrailer(Trailer *trailer)
{
  Answer::MutexGuard lock;
  if ( trailer )
  {
    Answer::MutexGuard lock(/* mutex member */);
    __k[0] = (*((int64_t (__fastcall **)(Trailer *))trailer->_vptr_Entity + 2))(trailer);
    *std::map<long,Trailer *>::operator[](&this->m_trailers, __k) = trailer;
    /* MutexGuard destroyed */
  }
}


void GameService::removeTrailer(Trailer *trailer)
{
  Answer::MutexGuard lock;
  if ( trailer )
  {
    Answer::MutexGuard lock(/* mutex member */);
    __x[0] = (*((int64_t (__fastcall **)(Trailer *))trailer->_vptr_Entity + 2))(trailer);
    std::map<long,Trailer *>::erase(&this->m_trailers, __x);
    /* MutexGuard destroyed */
  }
}


void GameService::saveAllPlayerToDB()
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nNowTime = Answer::DayTime::now();
  Answer::MutexGuard lock(&this->m_playerLock);
  for ( it._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&it) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
      break;
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it)->second;
    if ( player )
      Player::onLogout(player, 1, 0);
  }
  /* MutexGuard destroyed */
}


void GameService::onUserPayed(Answer::NetPacket *inPacket)
{
  std::pair<const long int,Player*> *v2; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  CharId_t Cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  if ( inPacket )
  {
    Cid = Answer::NetPacket::readInt64(inPacket);
    Answer::MutexGuard lock(&this->m_playerLock);
    it._M_node = std::map<long,Player *>::find(&this->m_players, &Cid)._M_node;
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
    {
      if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it)->second )
      {
        v2 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
        Player::addNetPacket(v2->second, inPacket, 8u);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::onAddUser( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  CPoolManager *v3; // rax
  Answer::MutexGuard lock;
  User *user;

  if ( inPacket )
  {
    cgindex = Answer::NetPacket::readInt16(inPacket);
    uid = Answer::NetPacket::readInt64(inPacket);
    sid = Answer::NetPacket::readInt32(inPacket);
    connid = MutiConn<GameService>::GetId(pConn);
    Answer::MutexGuard lock(&this->m_userLock);
    if ( (uint8_t)connid < 0x64u && cgindex > 0 && cgindex <= 9999 && uid > 0 && sid > 0 )
    {
      nIndex = getUserIndex(connid, cgindex);
      v3 = Answer::Singleton<CPoolManager>::instance();
      user = CPoolManager::pop<User>(v3);
      if ( user )
      {
        User::init(user, uid, sid);
        this->m_users[nIndex] = user;
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::qqToPlayer( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  TencentInfo info;
  Answer::MutexGuard lock;
  User *user;

  if ( inPacket )
  {
    cgindex = Answer::NetPacket::readInt16(inPacket);
    connid = MutiConn<GameService>::GetId(pConn);
    TencentInfo::TencentInfo(&info);
    TencentInfo::UnPacketInfo(&info, inPacket);
    Answer::MutexGuard lock(&this->m_userLock);
    nIndex = getUserIndex(connid, cgindex);
    user = this->m_users[cgindex];
    if ( user )
      User::SetTencentInfo(user, &info);
    /* MutexGuard destroyed */
    TencentInfo::~TencentInfo(&info);
  }
}


void GameService::onRemoveUser( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t v3; // bl
  GameService *v4; // rax

  CharId_t Cid; // rax
  void (__fastcall *v7)(Player *, _QWORD, _QWORD, _QWORD); // r12

  CPoolManager *v9; // rax
  CPoolManager *v10; // rax
  Answer::MutexGuard lock;
  User *user;
  Player *player;
  Answer::NetPacket *packet;

  if ( inPacket )
  {
    cgindex = Answer::NetPacket::readInt16(inPacket);
    connid = MutiConn<GameService>::GetId(pConn);
    if ( (uint8_t)connid < 0x64u && cgindex > 0 && cgindex <= 9999 )
    {
      Answer::MutexGuard lock(&this->m_userLock);
      nIndex = getUserIndex(connid, cgindex);
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
          CPoolManager::push<User>(v10, user);
          // goto removed
        }
        Answer::NetPacket::readInt64(inPacket);
        reason = Answer::NetPacket::readInt32(inPacket);
        param = Answer::NetPacket::readInt32(inPacket);
        if ( !StaticObj::getMap(player) )
        {
          Cid = Player::getCid(player);
          Answer::Logger::print(
            Answer::LogLevel::LOG_LEVEL_INFO,
            "GameService::onRemoveUser remove player with m_pMap == NULL, cid = %lld\n",
            Cid);
          v7 = (void (__fastcall *)(Player *, _QWORD, _QWORD, _QWORD))*((_QWORD *)player->_vptr_Entity + 35);
          Now = Unit::getNow(player);
          v7(player, (uint32_t)reason, (uint32_t)param, Now);
          GameService::removePlayer(this, player);
          if ( Player::getGateIndex(player) > 0 )
          {
            Player::setGateIndex(player, -1);
            if ( (*((unsigned __int8 (__fastcall **)(Player *))player->_vptr_Entity + 34))(player) )
            {
              if ( player )
                (*((void (__fastcall **)(Player *))player->_vptr_Entity + 1))(player);
            }
            else
            {
              v9 = Answer::Singleton<CPoolManager>::instance();
              CPoolManager::push<Player>(v9, player);
            }
          }
          // goto removed
        }
        v3 = Player::getConnId(player);
        v4 = Answer::Singleton<GameService>::instance();
        packet = GameService::popNetpacket(v4, v3, Answer::PackType::PACK_DISPATCH, 8u);
        if ( packet )
        {
          Answer::NetPacket::writeInt32(packet, reason);
          Answer::NetPacket::writeInt32(packet, param);
          WOffset = Answer::NetPacket::getWOffset(packet);
          Answer::NetPacket::setSize(packet, WOffset);
          Player::addNetPacket(player, packet, 0);
          Answer::NetPacket::destroy(packet);
          // goto removed
        }
      }
// label
      /* MutexGuard destroyed */
    }
  }
}


void GameService::onGMReloadItem( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  CfgData *v3; // rax

  v3 = Answer::Singleton<CfgData>::instance();
  CfgData::reload(v3);
}


void GameService::onSocialNetpacket( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  if ( inPacket )
  {
    cgindex = Answer::NetPacket::readInt32(inPacket);
    connid = MutiConn<GameService>::GetId(pConn);
    if ( (uint8_t)connid < 0x64u && cgindex > 0 && cgindex <= 9999 )
      GameService::onGameNetpacket(this, connid, cgindex, inPacket);
  }
}


void GameService::onGateDisconnect( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{


  int64_t v5; // rax

  Answer::NetPacket *packet;

  if ( pConn && inPacket )
  {
    strncpy(filename, "./log/GameConnErr.txt");
    memset(&filename[22], 0, 28);
    v3 = std::operator|(std::_Ios_Openmode::_S_out, std::_Ios_Openmode::_S_trunc);
    std::ofstream::basic_ofstream(v7, filename, (uint32_t)v3);
    v4 = Answer::DayTime::now();
    v5 = std::ostream::operator<<(v7, v4);
    std::ostream::operator<<(v5, &std::endl<char,std::char_traits<char>>);
    packet = GameService::popNetpacket(this, Answer::PackType::PACK_PROC, 0x4E2Eu);
    if ( packet )
    {
      Answer::NetPacket::writeInt32(packet, 1);
      WOffset = Answer::NetPacket::getWOffset(packet);
      Answer::NetPacket::setSize(packet, WOffset);
      Answer::TcpClient::sendPacket(pConn, packet);
    }
    std::ofstream::~ofstream(v7);
  }
}


void GameService::onEnterGame( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{

  DBService *v5; // rax
  Answer::MutexGuard lock;
  User *user;

  cid = Answer::NetPacket::readInt64(inPacket);
  line = Answer::NetPacket::readInt32(inPacket);
  reason = Answer::NetPacket::readInt32(inPacket);
  Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_DEBUG, "GameService::onEnterGame() cid=%lld, line=%d, reason=%d \n", cid, line, reason);
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
    DBService::loadPlayer(v5, connid, cgindex, uid, sid, reason);
  }
}


void GameService::onEnterGameRobot( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{

  const char *v6; // rax
  Answer::Random *v7; // rax
  PlayerDBData dbData;
  Answer::MutexGuard lock;
  Answer::NetPacket v13;

  if ( inPacket )
  {
    /* *(int64_t*)&v13.m_wOffset */ = Answer::NetPacket::readInt32(inPacket);
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
        PlayerDBData::PlayerDBData(&dbData);
        Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.cid = /* *(int64_t*)&v13.m_wOffset */;
        Answer::NetPacket::readUTF8(&v13, (bool)inPacket);
        v6 = (const char *)std::string::c_str((std::string *)&v13);
        snprintf(dbData.chr.data.name, 0x1Eu, v6);
                dbData.chr.data.sex = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.job = Answer::NetPacket::readInt32(inPacket);
        Answer::NetPacket::readInt32(inPacket);
        Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.level = 60;
        dbData.chr.data.mapid = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.x = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.y = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.hp = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.mp = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.kun_li = Answer::NetPacket::readInt32(inPacket);
        dbData.chr.data.head = Answer::NetPacket::readInt32(inPacket);
        id[0] = 10001;
        id[1] = 10006;
        id[2] = 10011;
        v7 = Answer::Singleton<Answer::Random>::instance();
        HIWORD(v13.m_refCount) = id[Answer::Random::generate(v7, 0, 0)];
        GameService::onPlayerLoaded(this, &dbData, 0, 0);
        PlayerDBData::~PlayerDBData(&dbData);
      }
    }
  }
}


void GameService::onGameNetpacket( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{
  Player *player;

  if ( inPacket )
  {
    player = GameService::getUserPlayer(this, connid, cgindex);
    if ( player )
      Player::addNetPacket(player, inPacket, 4u);
  }
}


void GameService::OnDBNetPacket( int8_t connid, int16_t cgindex, Answer::NetPacket *inPacket)
{
  Player *player;

  if ( inPacket )
  {
    player = GameService::getUserPlayer(this, connid, cgindex);
    if ( player )
      Player::addNetPacket(player, inPacket, 2u);
  }
}


void GameService::sendSocialAddPlayer(Player *player)
{
  int8_t ConnId; // bl

  CharId_t Cid; // rdx
  int64_t Uid; // rdx

  int8_t GMLevel; // dl

  int8_t v9; // cl
  Answer::NetPacket *packet;

  if ( player )
  {
    ConnId = Player::getConnId(player);
    packet = GameService::popNetpacket(this, ConnId, Answer::PackType::PACK_PROC, 0x4E34u);
    if ( packet )
    {
      GateIndex = Player::getGateIndex(player);
      Answer::NetPacket::writeInt32(packet, GateIndex);
      Cid = Player::getCid(player);
      Answer::NetPacket::writeInt64(packet, Cid);
      Uid = Player::getUid(player);
      Answer::NetPacket::writeInt64(packet, Uid);
      Sid = Player::getSid(player);
      Answer::NetPacket::writeInt32(packet, Sid);
      Answer::NetPacket::writeInt32(packet, this->m_line);
      GMLevel = Player::GetGMLevel(player);
      Answer::NetPacket::writeInt8(packet, GMLevel);
      WOffset = Answer::NetPacket::getWOffset(packet);
      Answer::NetPacket::setSize(packet, WOffset);
      v9 = Player::getConnId(player);
      sendPacket(v9, packet);
    }
  }
}


void GameService::sendSocialRemovePlayer(Player *player)
{
  int8_t ConnId; // bl
  CharId_t Cid; // rdx

  int8_t v5; // cl
  Answer::NetPacket *packet;

  if ( player )
  {
    ConnId = Player::getConnId(player);
    packet = GameService::popNetpacket(this, ConnId, Answer::PackType::PACK_PROC, 0x4E35u);
    if ( packet )
    {
      Cid = Player::getCid(player);
      Answer::NetPacket::writeInt64(packet, Cid);
      WOffset = Answer::NetPacket::getWOffset(packet);
      Answer::NetPacket::setSize(packet, WOffset);
      v5 = Player::getConnId(player);
      sendPacket(v5, packet);
    }
  }
}


void GameService::onSocialUpdatePetRank( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  CPetManager *v3; // rax

  v3 = Answer::Singleton<CPetManager>::instance();
  CPetManager::OnUpdatePetRank(v3, inPacket);
}


void GameService::onSocialUpdateFamilyInfo( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  FamilyManager *v3; // rax

  v3 = Answer::Singleton<FamilyManager>::instance();
  FamilyManager::OnUpdateFamilyInfo(v3, inPacket);
}


void GameService::onSocialInitPetRank( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  CPetManager *v3; // rax

  v3 = Answer::Singleton<CPetManager>::instance();
  CPetManager::OnInitPetRank(v3, inPacket);
}


void GameService::onSocialInitRankMirror( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  CRankMirror *v3; // rax

  v3 = Answer::Singleton<CRankMirror>::instance();
  CRankMirror::OnSocialUpdateRank(v3, inPacket);
}


void GameService::onSocialUpdateRankMirror( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  CRankMirror *v3; // rax

  v3 = Answer::Singleton<CRankMirror>::instance();
  CRankMirror::OnSocialUpdateRank(v3, inPacket);
}


void GameService::onSocialQueryPlayerInfo( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

  int8_t Id; // al
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  CharId_t cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *pPlayer;

  if ( pConn && inPacket )
  {
    cid = Answer::NetPacket::readInt64(inPacket);
    cgindex = Answer::NetPacket::readInt16(inPacket);
    Answer::MutexGuard lock(&this->m_playerLock);
    iter._M_node = std::map<long,Player *>::find(&this->m_players, &cid)._M_node;
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
    {
      pPlayer = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
      if ( pPlayer )
      {
        v3 = cgindex;
        Id = MutiConn<GameService>::GetId(pConn);
        Player::SendOtherPlayerInfo(pPlayer, Id, v3);
      }
    }
    /* MutexGuard destroyed */
  }
}


void GameService::onSocialCreateTeamDungeon( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  MapManager *v3; // rax

  MapManager *v5; // rax
  CharId_t memberId;
  CharIdList memberList;
  Dungeon *pDungeon;
  nDungeonId = Answer::NetPacket::readInt32(inPacket);
  v3 = Answer::Singleton<MapManager>::instance();
  pDungeon = MapManager::NewDungeon(v3, nDungeonId);
  if ( pDungeon )
  {
    std::list<long>::list(&memberList);
    for ( i = 0; i <= 3; ++i )
    {
      memberId = Answer::NetPacket::readInt64(inPacket);
      if ( memberId > 0 )
        std::list<long>::push_back(&memberList, &memberId);
    }
    Dungeon::InitTeamMember(pDungeon, &memberList);
    Dungeon::start(pDungeon, 0);
    RunnerId = Map::GetRunnerId(pDungeon);
    v5 = Answer::Singleton<MapManager>::instance();
    MapManager::PostMsg(v5, RunnerId, GameMsgCode::GMC_ADD_DUNGEON, pDungeon, 0, 0, 0, 0);
    std::list<long>::~list(&memberList);
  }
}


void GameService::onSocialSendSystemMail( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

  DBService *v4; // rax
  DBService *v5; // rax
  MemChrBag item;
  std::string mailParam;
  if ( pConn && inPacket )
  {
    nReceiver = Answer::NetPacket::readInt64(inPacket);
    nMailId = Answer::NetPacket::readInt32(inPacket);
    nReason = Answer::NetPacket::readInt32(inPacket);
    *(&item.endTime + 1) = 0;
    item.itemId = Answer::NetPacket::readInt32(inPacket);
    *(int32_t*)&item.itemClass = (uint8_t)Answer::NetPacket::readInt8(inPacket);
    item.itemCount = Answer::NetPacket::readInt32(inPacket);
    item.srcId = Answer::NetPacket::readInt64(inPacket);
    *(int32_t*)&item.bind = (uint8_t)Answer::NetPacket::readInt8(inPacket);
    item.endTime = Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readUTF8((Answer::NetPacket *const)&mailParam, (bool)inPacket);
    if ( item.itemId <= 0 || item.itemCount <= 0 )
    {
      v5 = Answer::Singleton<DBService>::instance();
      DBService::OnSendSysMail(v5, 0, nReceiver, nMailId, &mailParam, 0);
    }
    else
    {
      v3 = nReason;
      v4 = Answer::Singleton<DBService>::instance();
      DBService::OnSendSysMail(v4, 0, nReceiver, nMailId, &item, (ITEM_CHANGE_REASON)v3, &mailParam, 0);
    }
      }
}


void GameService::onSocialSendSystemMail2( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  MemChrBag *v3; // rbx
  MemChrBag *v4; // rbx
  MemChrBag *v5; // rbx

  DBService *v7; // rax
  DBService *v8; // rax
  MemChrBagVector vItem;
  std::string Param;
  std::string v15;
  if ( pConn && inPacket )
  {
    nReceiver = Answer::NetPacket::readInt64(inPacket);
    nMailId = Answer::NetPacket::readInt32(inPacket);
    nReason = Answer::NetPacket::readInt32(inPacket);
    nSize = Answer::NetPacket::readInt32(inPacket);
    if ( nSize <= 0 )
    {
                  v8 = Answer::Singleton<DBService>::instance();
      DBService::OnSendSysMail(v8, 0, nReceiver, nMailId, &v15, 0);
                }
    else
    {
      std::vector<MemChrBag>::vector(&vItem);
      v12.itemId = 0;
      v12.itemClass = 0;
      v12.itemCount = 0;
      v12.bind = 0;
      v12.endTime = 0;
      v12.srcId = 0;
      *(_OWORD *)&__x.itemId = *(_OWORD *)&v12.itemId;
      *(_OWORD *)&__x.endTime = *(unsigned int64_t *)&v12.endTime;
      std::vector<MemChrBag>::resize(&vItem, nSize, __x);
      for ( i = 0; i < nSize; ++i )
      {
        v3 = std::vector<MemChrBag>::operator[](&vItem, i);
        v3->itemClass = Answer::NetPacket::readInt8(inPacket);
        v4 = std::vector<MemChrBag>::operator[](&vItem, i);
        v4->itemId = Answer::NetPacket::readInt32(inPacket);
        v5 = std::vector<MemChrBag>::operator[](&vItem, i);
        v5->itemCount = Answer::NetPacket::readInt32(inPacket);
        std::vector<MemChrBag>::operator[](&vItem, i)->srcId = 0;
        std::vector<MemChrBag>::operator[](&vItem, i)->bind = 0;
        std::vector<MemChrBag>::operator[](&vItem, i)->endTime = 0;
      }
                  v6 = nReason;
      v7 = Answer::Singleton<DBService>::instance();
      DBService::OnSendSysMail(v7, 0, nReceiver, nMailId, &vItem, (ITEM_CHANGE_REASON)v6, &Param, 0);
                  std::vector<MemChrBag>::~vector(&vItem);
    }
  }
}


void GameService::onSetWorldLevel( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

  if ( pConn )
  {
    if ( inPacket )
    {
      Int32 = Answer::NetPacket::readInt32(inPacket);
      GameService::SetWorldLevel(this, Int32);
    }
  }
}


int32_t GameService::GetDropTimes(int32_t nGroupId)
{

  int32_t nGroupIda;

  std::_Rb_tree_iterator<std::pair<const int,int> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const int,int> > __x;

  nGroupIda = nGroupId;
  Answer::MutexGuard lock(&this->m_dropControlLock);
  iter._M_node = std::map<int,int>::find((std::map<int,int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda)._M_node;
  __x._M_node = std::map<int,int>::end(&this->m_dropControl)._M_node;
  if ( std::_Rb_tree_iterator<std::pair<int const,int>>::operator!=(&iter, &__x) )
    second = std::_Rb_tree_iterator<std::pair<int const,int>>::operator->(&iter)->second;
  else
    second = 0;
  /* MutexGuard destroyed */
  return second;
}


void GameService::AddDropTimes(int32_t nGroupId, int32_t nAddTimes)
{
  std::pair<const int,int> *v3; // rax
  GameService *v4; // rax
  CTimer *v5; // rax
  Answer::DBPool *v6; // rax

  DBService *v8; // rax
  int32_t nGroupIda;

  Answer::MySqlDBGuard db;
  std::_Rb_tree_iterator<std::pair<const int,int> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const int,int> > __x;

  nGroupIda = nGroupId;
  nTimes = nAddTimes;
  Answer::MutexGuard lock(&this->m_dropControlLock);
  iter._M_node = std::map<int,int>::find((std::map<int,int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda)._M_node;
  __x._M_node = std::map<int,int>::end(&this->m_dropControl)._M_node;
  if ( std::_Rb_tree_iterator<std::pair<int const,int>>::operator!=(&iter, &__x) )
  {
    v3 = std::_Rb_tree_iterator<std::pair<int const,int>>::operator->(&iter);
    v3->second += nAddTimes;
    nTimes = std::_Rb_tree_iterator<std::pair<int const,int>>::operator->(&iter)->second;
  }
  else
  {
    *std::map<int,int>::operator[]((std::map<int,int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda) = nAddTimes;
  }
  /* MutexGuard destroyed */
  v4 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v4) == 9 )
  {
    v5 = Answer::Singleton<CTimer>::instance();
    nowTime = CTimer::GetNow(v5);
    v6 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v6);
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
    Answer::MySqlDBGuard::excute(&db, szSQL);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
  }
  else
  {
    v7 = nGroupIda;
    v8 = Answer::Singleton<DBService>::instance();
    DBService::SaveDropTimes(v8, v7, nTimes);
  }
}


void GameService::UpdateDropTimes(int32_t nGroupId, int32_t nTimes)
{
  GameService *v3; // rax
  int32_t nGroupIda;

  Answer::MutexGuard lock;

  nGroupIda = nGroupId;
  v3 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v3) != 9 )
  {
    Answer::MutexGuard lock(&this->m_dropControlLock);
    *std::map<int,int>::operator[]((std::map<int,int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupIda) = nTimes;
    /* MutexGuard destroyed */
  }
}


void GameService::ResetDropTimes()
{
  GameService *v1; // rax
  Answer::DBPool *v2; // rax
  DBService *v3; // rax
  Answer::MySqlDBGuard db;
  Answer::MutexGuard lock;

  v1 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v1) == 9 )
  {
    v2 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, 0xFFFu, "DELETE FROM `cross_drop_control` WHERE `id`=%d", this->m_id);
    Answer::MySqlDBGuard::excute(&db, szSQL);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
  }
  else
  {
    v3 = Answer::Singleton<DBService>::instance();
    DBService::ClearDropTimes(v3);
  }
  Answer::MutexGuard lock(&this->m_dropControlLock);
  std::map<int,int>::clear(&this->m_dropControl);
  /* MutexGuard destroyed */
}


void GameService::InitDropTimes()
{
  Answer::DBPool *v1; // rax
  GameService *v2; // rax


  Answer::MySqlQuery *v7; // rax
  int *v8; // rax
  Answer::MySqlDBGuard db;
  int32_t nGroupId;
  Answer::MySqlQuery result;
  v1 = Answer::Singleton<Answer::DBPool>::instance();
  Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
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
  v7 = Answer::MySqlDBGuard::query(&db, szSQL);
  Answer::MySqlQuery::MySqlQuery(&result, v7);
  while ( !Answer::MySqlQuery::eof(&result) )
  {
    nGroupId = Answer::MySqlQuery::getIntValue(&result, "group_id", 0);
    nTimes = Answer::MySqlQuery::getIntValue(&result, "times", 0);
    v8 = std::map<int,int>::operator[]((std::map<int,int> *const)((char *)&loc_7A13D0 + (int64_t)this), &nGroupId);
    *v8 = nTimes;
    Answer::MySqlQuery::nextRow(&result);
  }
  Answer::MySqlQuery::~MySqlQuery(&result);
  Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}


void GameService::TeamDungeonEnterDungeon( Dungeon *pDungeon, const CharIdList *const lst)
{

  CfgData *v4; // rax
  const int64_t *v5; // rax


  CHuoYueDu *PlayerHuoYueDu; // rax


  GameService *v13; // rax
  void (__fastcall *v14)(Dungeon *, Player *, int64_t, _QWORD); // r13

  int64_t x; // rdx
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  std::_List_const_iterator<long int> eiter;
  std::_List_const_iterator<long int> iter;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  CfgDungeon *pCfgDungeon;
  Player *pPlayer;
  Map *pMap;

  DungeonId = Dungeon::getDungeonId(pDungeon);
  v4 = Answer::Singleton<CfgData>::instance();
  pCfgDungeon = CfgData::getDungeon(v4, DungeonId);
  if ( pDungeon && pCfgDungeon )
  {
    iter._M_node = std::list<long>::begin(lst)._M_node;
    eiter._M_node = std::list<long>::end(lst)._M_node;
    while ( std::_List_const_iterator<long>::operator!=(&iter, &eiter) )
    {
      Answer::MutexGuard lock(&this->m_playerLock);
      v5 = std::_List_const_iterator<long>::operator*(&iter);
      it._M_node = std::map<long,Player *>::find(&this->m_players, v5)._M_node;
      __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
      if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
      {
        pPlayer = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it)->second;
        if ( pPlayer )
        {
          Player::setOldPosition(pPlayer);
          pMap = StaticObj::getMap(pPlayer);
          if ( pMap )
          {
            Player::broadcastLeave(pPlayer);
            (*((void (__fastcall **)(Map *, Player *, _QWORD))pMap->_vptr_Map + 16))(pMap, pPlayer, 0);
          }
          DungeonGroupId = Dungeon::getDungeonGroupId(pDungeon);
          v7 = Player::getRecord(pPlayer, DungeonGroupId) + 1;
          v8 = Dungeon::getDungeonGroupId(pDungeon);
          Player::updateRecord(pPlayer, v8, v7);
          v9 = Dungeon::getDungeonGroupId(pDungeon);
          PlayerHuoYueDu = Player::GetPlayerHuoYueDu(pPlayer);
          CHuoYueDu::AddHuoYueDuRecord(PlayerHuoYueDu, 2, v9, 0);
          v11 = Dungeon::getDungeonId(pDungeon);
          GateIndex = Player::getGateIndex(pPlayer);
          LOBYTE(v9) = Player::getConnId(pPlayer);
          v13 = Answer::Singleton<GameService>::instance();
          GameService::replySuccess(v13, v9, GateIndex, 0x12u, v11);
          v14 = (void (__fastcall *)(Dungeon *, Player *, int64_t, _QWORD))*((_QWORD *)pDungeon->_vptr_Map + 15);
          y = Dungeon::GetCfgDungeon(pDungeon)->y;
          x = (uint32_t)Dungeon::GetCfgDungeon(pDungeon)->x;
          v14(pDungeon, pPlayer, x, y);
        }
      }
      /* MutexGuard destroyed */
      std::_List_const_iterator<long>::operator++(&iter);
    }
  }
}


void GameService::OnUpdateMail(Answer::NetPacket *inPacket)
{
  std::pair<const long int,Player*> *v2; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  CharId_t Cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  if ( inPacket )
  {
    Cid = Answer::NetPacket::readInt64(inPacket);
    Answer::MutexGuard lock(&this->m_playerLock);
    it._M_node = std::map<long,Player *>::find(&this->m_players, &Cid)._M_node;
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
    {
      if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it)->second )
      {
        v2 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
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
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;
  Answer::NetPacket *packet;

  for ( iter._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::begin(&this->m_mConn)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator++(&iter) )
  {
    __x._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::end(&this->m_mConn)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator!=(&iter, &__x) )
      break;
    pConn = std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
      packet = Answer::TcpClient::popNetpacket(pConn);
      if ( packet )
      {
        Answer::NetPacket::writeInt32(packet, this->m_line);
        WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Answer::NetPacket::setProc(packet, 0x4E42u);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_DISPATCH);
        Answer::TcpClient::sendPacket(pConn, packet);
      }
    }
  }
}


void GameService::OnReceiveDaTiRank( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  GameService *v3; // rax
  int8_t v4; // bl
  DBService *v5; // rax
  GameService *v6; // rax


  int8_t v9; // bl
  DBService *v10; // rax
  int8_t v11; // bl
  DBService *v12; // rax
  int8_t v13; // bl
  DBService *v14; // rax
  int8_t v15; // bl
  DBService *v16; // rax
  int8_t v17; // bl
  DBService *v18; // rax
  int8_t v19; // bl
  CFestivalDoubleEleven *v20; // rax
  CTimer *v21; // rax
  int8_t v22; // bl
  DBService *v23; // rax
  LogActivity logActivity;
  std::string Name;
  std::string Param;
  std::string v29;
  std::string v31;
  std::string v33;
  std::string v35;
  std::string v37;
  Answer::NetPacket *packet;

  if ( pConn && inPacket )
  {
    nSize = Answer::NetPacket::readInt32(inPacket);
    for ( i = 0; ; ++i )
    {
      if ( i >= nSize )
        return;
      CharId = Answer::NetPacket::readInt64(inPacket);
      Answer::NetPacket::readUTF8((Answer::NetPacket *const)&Name, (bool)inPacket);
      Index = Answer::NetPacket::readInt32(inPacket);
      v3 = Answer::Singleton<GameService>::instance();
      connid = GameService::GetConnId(v3, CharId);
      switch ( Index )
      {
        case 1:
                              v4 = connid;
          v5 = Answer::Singleton<DBService>::instance();
          DBService::OnSendSysMail(v5, v4, CharId, 6203, &Param, 0);
                              v6 = Answer::Singleton<GameService>::instance();
          packet = GameService::popNetpacket(v6, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
          if ( !packet )
          {
            v7 = 0;
            // goto removed
          }
          Answer::NetPacket::writeInt32(packet, 28);
          Answer::NetPacket::writeUTF8(packet, &Name);
          Answer::NetPacket::writeInt64(packet, CharId);
          WOffset = Answer::NetPacket::getWOffset(packet);
          Answer::NetPacket::setSize(packet, WOffset);
          GameService::worldBroadcast(this, 0, packet);
          break;
        case 2:
                              v9 = connid;
          v10 = Answer::Singleton<DBService>::instance();
          DBService::OnSendSysMail(v10, v9, CharId, 6204, &v29, 0);
                              break;
        case 3:
                              v11 = connid;
          v12 = Answer::Singleton<DBService>::instance();
          DBService::OnSendSysMail(v12, v11, CharId, 6205, &v31, 0);
                              break;
        default:
          if ( Index > 10 )
          {
            if ( Index > 20 )
            {
                                          v17 = connid;
              v18 = Answer::Singleton<DBService>::instance();
              DBService::OnSendSysMail(v18, v17, CharId, 6206, &v37, 0);
                                        }
            else
            {
                                          v15 = connid;
              v16 = Answer::Singleton<DBService>::instance();
              DBService::OnSendSysMail(v16, v15, CharId, 6269, &v35, 0);
                                        }
          }
          else
          {
                                    v13 = connid;
            v14 = Answer::Singleton<DBService>::instance();
            DBService::OnSendSysMail(v14, v13, CharId, 6268, &v33, 0);
                                  }
          break;
      }
      v19 = connid;
      v20 = Answer::Singleton<CFestivalDoubleEleven>::instance();
      CFestivalDoubleEleven::SendDaTiReward(v20, v19, Index, CharId);
      /* *(int64_t*)&logActivity.actid */ = 0x500000000LL;
      /* *(int64_t*)&logActivity.time */ = 0;
      logActivity.param = 0;
      logActivity.cid = CharId;
      v21 = Answer::Singleton<CTimer>::instance();
      logActivity.time = CTimer::GetNow(v21);
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


void GameService::OnReceiveDaTiResult( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t v3; // bl
  std::pair<const long int,Player*> *v4; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > it;
  Answer::MutexGuard lock;
  int64_t Cid;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  if ( pConn && inPacket )
  {
    Cid = Answer::NetPacket::readInt64(inPacket);
    Index = Answer::NetPacket::readInt32(inPacket);
    Result = Answer::NetPacket::readInt8(inPacket);
    Answer::MutexGuard lock(&this->m_playerLock);
    it._M_node = std::map<long,Player *>::find(&this->m_players, &Cid)._M_node;
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&it, &__x) )
    {
      v3 = Result;
      v4 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&it);
      Player::AddDaTiExp(v4->second, Index, v3);
    }
    /* MutexGuard destroyed */
  }
}


void GameService::SendServerDiffToGlobal()
{
  CfgData *v1; // rax


  Answer::NetPacket *packet;

  packet = GameService::popNetpacket(this, 0, Answer::PackType::PACK_PROC, 0x4E49u);
  if ( packet )
  {
    v1 = Answer::Singleton<CfgData>::instance();
    ServerDiffDay = CfgData::getServerDiffDay(v1, SERVER_TYPE::SVT_NORMAL);
    Answer::NetPacket::writeInt32(packet, ServerDiffDay);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    sendPacket(0, packet);
  }
}


void GameService::UpdateCityWarTitle(FamilyId_t OldFamilyId, FamilyId_t NewFamilyId)
{
  std::pair<const long int,Player*> *v3; // rax
  std::pair<const long int,Player*> *v4; // rax
  CExtCharTitle *CharTitle; // rax
  std::pair<const long int,Player*> *v6; // rax
  std::pair<const long int,Player*> *v7; // rax

  std::pair<const long int,Player*> *v9; // rax
  CExtCharTitle *v10; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    v3 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
    if ( Player::getFamilyId(v3->second) == OldFamilyId )
    {
      v4 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
      CharTitle = Player::GetCharTitle(v4->second);
      CExtCharTitle::RemoveTitle(CharTitle, 1, 0);
    }
    v6 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
    if ( Player::getFamilyId(v6->second) == NewFamilyId )
    {
      v7 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
      FamilyPosition = Player::getFamilyPosition(v7->second);
      v9 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
      v10 = Player::GetCharTitle(v9->second);
      CExtCharTitle::CheckAddTitle(v10, 1, FamilyPosition);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::UpdateCityActState(FamilyId_t FamilyId, int8_t ActState)
{
  std::pair<const long int,Player*> *v3; // rax
  std::pair<const long int,Player*> *v4; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    v3 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
    if ( Player::getFamilyId(v3->second) == FamilyId )
    {
      v4 = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter);
      Player::SetActState(v4->second, ActState);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::SetFamilyMemberNeedSync(FamilyId_t nFamilyId)
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player && Player::getFamilyId(player) == nFamilyId )
      Player::SetNeedSyncAround(player);
  }
  /* MutexGuard destroyed */
}


void GameService::GetFamilyMemberInMap( FamilyId_t nFamilyId, int32_t nMapId, PlayerList *const pList, CharIdList *const farList, int32_t nLevel)
{
  Player *player;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > v14;
  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter) )
  {
    v14._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &v14) )
      break;
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player
      && Player::getFamilyId(player) == nFamilyId
      && (*((int (__fastcall **)(Player *))player->_vptr_Entity + 9))(player) > nLevel )
    {
      if ( StaticObj::getMapId(player) == nMapId )
      {
        std::list<Player *>::push_back(pList, &player);
      }
      else
      {
        __x[0] = Player::getCid(player);
        std::list<long>::push_back(farList, __x);
      }
    }
  }
  /* MutexGuard destroyed */
}


void GameService::onUpdateActivityState( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  int8_t Int8; // bl
  CActivityManager *v4; // rax
  if ( pConn )
  {
    if ( inPacket )
    {
      nId = Answer::NetPacket::readInt32(inPacket);
      Int8 = Answer::NetPacket::readInt8(inPacket);
      v4 = Answer::Singleton<CActivityManager>::instance();
      CActivityManager::UpdateActivityState(v4, nId, Int8);
    }
  }
}


void GameService::OnSocialCityMasterBanChat( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  int *v3; // rax
  Answer::MutexGuard lock;
  CharId_t nCharId;
  if ( pConn )
  {
    if ( inPacket )
    {
      nCharId = Answer::NetPacket::readInt64(inPacket);
      nEndTime = Answer::NetPacket::readInt32(inPacket);
      Answer::MutexGuard lock(&this->m_cityMasterBanChatLock);
      v3 = std::map<long,int>::operator[](&this->m_mCityMasterBanChatMap, &nCharId);
      *v3 = nEndTime;
      /* MutexGuard destroyed */
    }
  }
}


int32_t GameService::GetCityMasterBanChatTime(CharId_t nCharId)
{

  CharId_t nCharIda;

  Answer::MutexGuard lock;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_cityMasterBanChatLock);
  v2 = *std::map<long,int>::operator[](&this->m_mCityMasterBanChatMap, &nCharIda);
  /* MutexGuard destroyed */
  return v2;
}


void GameService::KickUser(CharId_t cid, int32_t opWay)
{
  int8_t ConnId; // al
  int64_t Uid; // rax


  int8_t v7; // al
  CharId_t cida;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;
  Answer::NetPacket *packet;

  cida = cid;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter._M_node = std::map<long,Player *>::find(&this->m_players, &cida)._M_node;
  __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
  if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator==(&iter, &__x) )
  {
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player )
    {
      ConnId = Player::getConnId(player);
      packet = GameService::popNetpacket(this, ConnId, Answer::PackType::PACK_PROC, 0x4E2Au);
      if ( packet )
      {
        Uid = Player::getUid(player);
        Answer::NetPacket::writeInt64(packet, Uid);
        Sid = Player::getSid(player);
        Answer::NetPacket::writeInt32(packet, Sid);
        Answer::NetPacket::writeInt32(packet, opWay);
        WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
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
  std::map<int8_t,GameService::ConnType*>::ConnType *pConn;
  Answer::NetPacket *packet;

  for ( iter._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::begin(&this->m_mConn)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator++(&iter) )
  {
    __x._M_node = std::map<int8_t,MutiConn<GameService> *,std::less<int8_t>,std::allocator<std::pair<int8_t const,MutiConn<GameService> *>>>::end(&this->m_mConn)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator!=(&iter, &__x) )
      break;
    pConn = std::_Rb_tree_iterator<std::pair<int8_t const,MutiConn<GameService> *>>::operator->(&iter)->second;
    if ( pConn )
    {
      packet = Answer::TcpClient::popNetpacket(pConn);
      if ( packet )
      {
        Answer::NetPacket::writeInt32(packet, opWay);
        WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_PROC);
        Answer::NetPacket::setProc(packet, 0x4E2Bu);
        Answer::TcpClient::sendPacket(pConn, packet);
      }
    }
  }
}


void GameService::onCheckTitle(CharId_t nCharId, int8_t nType, int32_t nParam)
{
  CExtCharTitle *CharTitle; // rax
  CharId_t nCharIda;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter._M_node = std::map<long,Player *>::find(&this->m_players, &nCharIda)._M_node;
  __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
  if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator==(&iter, &__x) )
  {
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player )
    {
      CharTitle = Player::GetCharTitle(player);
      CExtCharTitle::CheckAddTitle(CharTitle, nType, nParam);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::onCheckTeShuTitle(CharId_t nCharId, int8_t nType, int32_t nParam)
{
  Answer::DBPool *v4; // rax

  CExtCharTitle *CharTitle; // rax
  CharId_t nCharIda;

  Answer::MySqlDBGuard db;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter._M_node = std::map<long,Player *>::find(&this->m_players, &nCharIda)._M_node;
  __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
  if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator==(&iter, &__x) )
  {
    v4 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v4);
    Time = Answer::DayTime::now() + 86340;
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
    Answer::MySqlDBGuard::excute(&db, szSQL);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
  }
  else
  {
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player )
    {
      Now = Unit::getNow(player);
      Player::updateRecord(player, nParam + 31301, Now + 86340);
      CharTitle = Player::GetCharTitle(player);
      CExtCharTitle::CheckAddTitle(CharTitle, nType, nParam);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::onRemoveTitle(CharId_t nCharId, int8_t nType)
{
  CExtCharTitle *CharTitle; // rax
  CharId_t nCharIda;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  nCharIda = nCharId;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter._M_node = std::map<long,Player *>::find(&this->m_players, &nCharIda)._M_node;
  __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
  if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator==(&iter, &__x) )
  {
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player )
    {
      CharTitle = Player::GetCharTitle(player);
      CExtCharTitle::RemoveTitle(CharTitle, nType, 0);
    }
  }
  /* MutexGuard destroyed */
}


void GameService::requestWorldLevel()
{

  Answer::NetPacket *packet;

  packet = GameService::popNetpacket(this, 0, Answer::PackType::PACK_PROC, 0x4E4Bu);
  if ( packet )
  {
    Answer::NetPacket::writeInt32(packet, 0);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    sendPacket(0, packet);
  }
}


void GameService::onRequestDropRecord( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{

  int8_t Id; // bl
  CWorldBoss *v5; // rax

  if ( pConn )
  {
    if ( inPacket )
    {
      Int16 = Answer::NetPacket::readInt16(inPacket);
      Id = MutiConn<GameService>::GetId(pConn);
      v5 = Answer::Singleton<CWorldBoss>::instance();
      CWorldBoss::SendDropRecord(v5, Id, Int16);
    }
  }
}


void GameService::InitServerBattle()
{
  GameService *v1; // rax
  DBService *v2; // rax
  Answer::DBPool *v3; // rax
  Answer::MySqlQuery *v4; // rax
  const char *StringValue; // rax
  Answer::MySqlDBGuard db;
  Answer::MySqlQuery result;

  v1 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v1) == 9 )
  {
    v2 = Answer::Singleton<DBService>::instance();
    DBService::onLoadServerBattle(v2);
  }
  else
  {
    v3 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v3);
    v4 = Answer::MySqlDBGuard::query(&db, "SELECT * FROM `sys_server_config` WHERE `name`='BATTLE'");
    Answer::MySqlQuery::MySqlQuery(&result, v4);
    if ( !Answer::MySqlQuery::eof(&result) )
    {
      StringValue = Answer::MySqlQuery::getStringValue(&result, "value", byte_8DA6C0);
      this->m_Battle = atoi(StringValue);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
  }
}


void GameService::SetServerBattle(int32_t Battle)
{
  if ( this->m_Battle < Battle )
    this->m_Battle = Battle;
}


void GameService::InitMoYuShiJieRecord()
{
  GameService *v1; // rax
  Answer::DBPool *v2; // rax
  Answer::MySqlQuery *v3; // rax
  const char *StringValue; // rax
  Answer::MySqlQuery *v5; // rax
  const char *v6; // rax
  LogMoYuShiJieReward record;
  Answer::MySqlDBGuard db;
  Answer::MySqlQuery result;
  v1 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v1) != 9 )
  {
    v2 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
    nSize = 0;
    v3 = Answer::MySqlDBGuard::query(
           &db,
           "SELECT * FROM `log_moyushijie_reward` WHERE `special`=1 ORDER BY `time` DESC LIMIT 5");
    Answer::MySqlQuery::MySqlQuery(&result, v3);
    while ( !Answer::MySqlQuery::eof(&result) )
    {
      memset(&record, 0, sizeof(record));
      record.cid = Answer::MySqlQuery::getInt64Value(&result, "cid", 0);
      StringValue = Answer::MySqlQuery::getStringValue(&result, "name", byte_8DA6C0);
      snprintf(record.name, 0x1Eu, StringValue);
      record.dungeonid = Answer::MySqlQuery::getIntValue(&result, "dungeonid", 0);
      record.itemid = Answer::MySqlQuery::getIntValue(&result, "itemid", 0);
      record.itemclass = Answer::MySqlQuery::getIntValue(&result, "itemclass", 0);
      record.itemcount = Answer::MySqlQuery::getIntValue(&result, "itemcount", 0);
      record.time = Answer::MySqlQuery::getIntValue(&result, "time", 0);
      record.special = Answer::MySqlQuery::getIntValue(&result, "special", 0);
      std::list<LogMoYuShiJieReward>::push_front(&this->m_lMYSJRecordSpecial, &record);
      if ( ++nSize > 4 )
        break;
      Answer::MySqlQuery::nextRow(&result);
    }
    v5 = Answer::MySqlDBGuard::query(
           &db,
           "SELECT * FROM `log_moyushijie_reward` WHERE `special`=0 ORDER BY `time` DESC LIMIT 30");
    Answer::MySqlQuery::operator=(&result, v5);
    while ( !Answer::MySqlQuery::eof(&result) )
    {
      memset(&record, 0, sizeof(record));
      record.cid = Answer::MySqlQuery::getInt64Value(&result, "cid", 0);
      v6 = Answer::MySqlQuery::getStringValue(&result, "name", byte_8DA6C0);
      snprintf(record.name, 0x1Eu, v6);
      record.dungeonid = Answer::MySqlQuery::getIntValue(&result, "dungeonid", 0);
      record.itemid = Answer::MySqlQuery::getIntValue(&result, "itemid", 0);
      record.itemclass = Answer::MySqlQuery::getIntValue(&result, "itemclass", 0);
      record.itemcount = Answer::MySqlQuery::getIntValue(&result, "itemcount", 0);
      record.time = Answer::MySqlQuery::getIntValue(&result, "time", 0);
      record.special = Answer::MySqlQuery::getIntValue(&result, "special", 0);
      std::list<LogMoYuShiJieReward>::push_front(&this->m_lMYSJRecord, &record);
      if ( ++nSize > 29 )
        break;
      Answer::MySqlQuery::nextRow(&result);
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
  }
}


void GameService::SendMoYuShiJieRecord(Player *player)
{
  int8_t ConnId; // bl


  const LogMoYuShiJieReward *v5; // rax
  const LogMoYuShiJieReward *v6; // rax
  const LogMoYuShiJieReward *v7; // rax
  const LogMoYuShiJieReward *v8; // rax
  const LogMoYuShiJieReward *v9; // rax
  const LogMoYuShiJieReward *v10; // rax
  const LogMoYuShiJieReward *v11; // rax
  const LogMoYuShiJieReward *v12; // rax
  const LogMoYuShiJieReward *v13; // rax
  const LogMoYuShiJieReward *v14; // rax
  const LogMoYuShiJieReward *v15; // rax
  const LogMoYuShiJieReward *v16; // rax
  const LogMoYuShiJieReward *v17; // rax
  const LogMoYuShiJieReward *v18; // rax
  const LogMoYuShiJieReward *v19; // rax
  const LogMoYuShiJieReward *v20; // rax


  int8_t v23; // bl
  std::_List_const_iterator<LogMoYuShiJieReward> iter_0;
  std::_List_const_iterator<LogMoYuShiJieReward> iter;
  Answer::MutexGuard lock;
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
    packet = GameService::popNetpacket(this, ConnId, Answer::PackType::PACK_DISPATCH, 0x2ED7u);
    if ( packet )
    {
      Answer::MutexGuard lock(&this->m_mysjRecordLock);
      v3 = std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecord);
      v4 = std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecordSpecial);
      Answer::NetPacket::writeInt16(packet, v3 + v4);
      __x._M_node = std::list<LogMoYuShiJieReward>::begin(&this->m_lMYSJRecordSpecial)._M_node;
      std::_List_const_iterator<LogMoYuShiJieReward>::_List_const_iterator(&iter, &__x);
      while ( 1 )
      {
        v29._M_node = std::list<LogMoYuShiJieReward>::end(&this->m_lMYSJRecordSpecial)._M_node;
        std::_List_const_iterator<LogMoYuShiJieReward>::_List_const_iterator(&v28, &v29);
        if ( !std::_List_const_iterator<LogMoYuShiJieReward>::operator!=(&iter, &v28) )
          break;
        v5 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt64(packet, v5->cid);
                v6 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
                Answer::NetPacket::writeUTF8(packet, &val);
                        v7 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt32(packet, v7->dungeonid);
        v8 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt32(packet, v8->itemid);
        v9 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt8(packet, v9->itemclass);
        v10 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt32(packet, v10->itemcount);
        v11 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt32(packet, v11->time);
        v12 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter);
        Answer::NetPacket::writeInt8(packet, v12->special);
        std::_List_const_iterator<LogMoYuShiJieReward>::operator++(&iter);
      }
      v32._M_node = std::list<LogMoYuShiJieReward>::begin(&this->m_lMYSJRecord)._M_node;
      std::_List_const_iterator<LogMoYuShiJieReward>::_List_const_iterator(&iter_0, &v32);
      while ( 1 )
      {
        v34._M_node = std::list<LogMoYuShiJieReward>::end(&this->m_lMYSJRecord)._M_node;
        std::_List_const_iterator<LogMoYuShiJieReward>::_List_const_iterator(&v33, &v34);
        if ( !std::_List_const_iterator<LogMoYuShiJieReward>::operator!=(&iter_0, &v33) )
          break;
        v13 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt64(packet, v13->cid);
                v14 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
                Answer::NetPacket::writeUTF8(packet, &v35);
                        v15 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt32(packet, v15->dungeonid);
        v16 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt32(packet, v16->itemid);
        v17 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt8(packet, v17->itemclass);
        v18 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt32(packet, v18->itemcount);
        v19 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt32(packet, v19->time);
        v20 = std::_List_const_iterator<LogMoYuShiJieReward>::operator->(&iter_0);
        Answer::NetPacket::writeInt8(packet, v20->special);
        std::_List_const_iterator<LogMoYuShiJieReward>::operator++(&iter_0);
      }
      /* MutexGuard destroyed */
      WOffset = Answer::NetPacket::getWOffset(packet);
      Answer::NetPacket::setSize(packet, WOffset);
      GateIndex = Player::getGateIndex(player);
      v23 = Player::getConnId(player);
      GameService::sendPacketTo(this, v23, GateIndex, packet);
    }
  }
}


void GameService::AddMoYuShiJieRecord(const LogMoYuShiJieReward *const logReward)
{
  GameService *v2; // rax
  size_t v3; // rbx
  size_t v4; // rbx
  Answer::MutexGuard lock_0;
  Answer::MutexGuard lock;

  v2 = Answer::Singleton<GameService>::instance();
  if ( GameService::getLine(v2) != 9 && logReward->special >= 0 )
  {
    if ( logReward->special )
    {
      Answer::MutexGuard lock_0(&this->m_mysjRecordLock);
      std::list<LogMoYuShiJieReward>::push_front(&this->m_lMYSJRecordSpecial, logReward);
      if ( std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecordSpecial) <= 5 )
      {
        v4 = std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecordSpecial);
        if ( v4 + std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecord) > 0x1E )
          std::list<LogMoYuShiJieReward>::pop_front(&this->m_lMYSJRecord);
      }
      else
      {
        std::list<LogMoYuShiJieReward>::pop_back(&this->m_lMYSJRecordSpecial);
      }
      /* MutexGuard destroyed */
    }
    else
    {
      Answer::MutexGuard lock(&this->m_mysjRecordLock);
      std::list<LogMoYuShiJieReward>::push_front(&this->m_lMYSJRecord, logReward);
      v3 = std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecord);
      if ( v3 + std::list<LogMoYuShiJieReward>::size(&this->m_lMYSJRecordSpecial) > 0x1E )
        std::list<LogMoYuShiJieReward>::pop_back(&this->m_lMYSJRecord);
      /* MutexGuard destroyed */
    }
  }
}


void GameService::ResetFestivalData(int32_t nVersion)
{
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player )
      Player::ResetFestivalData(player, nVersion);
  }
  /* MutexGuard destroyed */
}


void GameService::UpdateTencentInfo(CharId_t cid, const TencentInfo *const info)
{
  CExtCharTencent *CharTencent; // rax
  CharId_t cida;

  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  cida = cid;
  Answer::MutexGuard lock(&this->m_playerLock);
  iter._M_node = std::map<long,Player *>::find(&this->m_players, &cida)._M_node;
  __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
  if ( std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
  {
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
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


  char *Buffer; // rcx

  Answer::NetPacket *packet;

  if ( inPacket )
  {
    packet = GameService::popNetpacket(this, connid, Answer::PackType::PACK_DISPATCH, 0x4E4Fu);
    if ( packet )
    {
      Answer::NetPacket::writeInt64(packet, nFamilyId);
      Proc = Answer::NetPacket::getProc(inPacket);
      Answer::NetPacket::writeUInt16(packet, Proc);
      Size = Answer::NetPacket::getSize(inPacket);
      Buffer = Answer::NetPacket::getBuffer(inPacket);
      Answer::NetPacket::write(packet, Buffer, Size);
      WOffset = Answer::NetPacket::getWOffset(packet);
      Answer::NetPacket::setSize(packet, WOffset);
      sendPacket(connid, packet);
    }
    Answer::NetPacket::destroy(inPacket);
  }
}


void GameService::onSocialInitFamilyInfo( std::map<int8_t,ConnType*>::ConnType *pConn, Answer::NetPacket *inPacket)
{
  FamilyManager *v3; // rax

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
  GameService *v3; // rax

  GameService *v5; // rax
  Answer::NetPacket *packet;

  v3 = Answer::Singleton<GameService>::instance();
  packet = GameService::popNetpacket(v3, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
  if ( packet )
  {
    Answer::NetPacket::writeInt32(packet, 334);
    Answer::NetPacket::writeUTF8(packet, strName);
    Answer::NetPacket::writeInt64(packet, nCharId);
    WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    v5 = Answer::Singleton<GameService>::instance();
    GameService::worldBroadcast(v5, packet);
  }
}


void GameService::broadCastKiaFuHuoDongIcon()
{
  CKaiFuHuoDong *v1; // rax
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > iter;
  Answer::MutexGuard lock;
  std::_Rb_tree_iterator<std::pair<const long int,Player*> > __x;
  Player *player;

  Answer::MutexGuard lock(&this->m_playerLock);
  for ( iter._M_node = std::map<long,Player *>::begin(&this->m_players)._M_node;
        ;
        std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator++(&iter) )
  {
    __x._M_node = std::map<long,Player *>::end(&this->m_players)._M_node;
    if ( !std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator!=(&iter, &__x) )
      break;
    player = std::_Rb_tree_iterator<std::pair<long const,Player *>>::operator->(&iter)->second;
    if ( player )
    {
      v1 = Answer::Singleton<CKaiFuHuoDong>::instance();
      CKaiFuHuoDong::SendKaiFuHuoDongIcon(v1, player);
    }
  }
  /* MutexGuard destroyed */
}

