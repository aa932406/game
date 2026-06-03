#include "common.h"
#include "Other/CWorldBoss.h"

#include <cstring>
#include <algorithm>

CWorldBoss::CWorldBoss()
{
    m_lastUpdateTick = 0;
}

CWorldBoss::~CWorldBoss()
{
}

void CWorldBoss::Init(int32_t line)
{
    if (line == 9)
        return;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);

    CfgData *pCfg = CfgData::instance();
    BossInfoMap *pBossInfoMap = CfgData::GetBossInfoMap(pCfg);
    BossInfoMap BossMap(*pBossInfoMap);

    for (auto &pair : BossMap)
    {
        int32_t first = pair.first;
        CfgMapMonster *pCfgMapMonster = CfgData::GetMapMonsterInfo(pCfg, first);
        if (!pCfgMapMonster)
            continue;

        int32_t monsterid = pCfgMapMonster->monsterid;
        CfgMonster *pMonster = CfgData::getMonster(pCfg, monsterid);
        if (!pMonster)
            continue;

        WorldBossInfo stu;
        stu.nBossId = first;
        stu.nMid = pMonster->mid;
        stu.nBossType = pMonster->boss_sign;
        stu.nMapId = pCfgMapMonster->mapid;
        stu.nLevel = 1;

        if (stu.nBossType == 3)
        {
            stu.nState = 3;
            stu.nReviveTime = 0;
        }
        else
        {
            stu.nState = 0;
            int32_t ReviveTime = GetBossRevieTime(stu.nBossId);
            if (ReviveTime <= 0)
            {
                int32_t boss_sign = pMonster->boss_sign;
                int32_t revive_time = pMonster->revive_time;
                ReviveTime = CfgData::GetMonsterReviveTime(pCfg, revive_time, boss_sign) / 1000;
            }
            stu.nReviveTime = nNowTime + ReviveTime;
        }

        m_mBossMap[stu.nBossId] = stu;
    }

    InitDBInfo();
    InitDropRecord();
}

void CWorldBoss::InitDBInfo()
{
    Answer::DBPool *pDB = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(pDB);
    Answer::MySqlQuery result = db.query("SELECT * FROM `mem_world_boss_info`");

    while (!result.eof())
    {
        int32_t nBossId = result.getIntValue("id", 0);
        auto it = m_mBossMap.find(nBossId);
        if (it != m_mBossMap.end())
        {
            it->second.nLevel = result.getIntValue("level", 0);
            it->second.nExp = result.getIntValue("exp", 0);
            it->second.vKiller[0].nCharId = result.getInt64Value("killer1", 0);
            it->second.vKiller[0].strName = result.getStringValue("killer_name1", "");
            it->second.vKiller[0].nTime = result.getIntValue("kill_time1", 0);
            it->second.vKiller[1].nCharId = result.getInt64Value("killer2", 0);
            it->second.vKiller[1].strName = result.getStringValue("killer_name2", "");
            it->second.vKiller[1].nTime = result.getIntValue("kill_time2", 0);
            it->second.vKiller[2].nCharId = result.getInt64Value("killer3", 0);
            it->second.vKiller[2].strName = result.getStringValue("killer_name3", "");
            it->second.vKiller[2].nTime = result.getIntValue("kill_time3", 0);
            it->second.vKiller[3].nCharId = result.getInt64Value("killer4", 0);
            it->second.vKiller[3].strName = result.getStringValue("killer_name4", "");
            it->second.vKiller[3].nTime = result.getIntValue("kill_time4", 0);
            it->second.vKiller[4].nCharId = result.getInt64Value("killer5", 0);
            it->second.vKiller[4].strName = result.getStringValue("killer_name5", "");
            it->second.vKiller[4].nTime = result.getIntValue("kill_time5", 0);
        }
        result.nextRow();
    }
}

Position CWorldBoss::GetRevivePos(int32_t BossId)
{
    Position Pos(0, 0);
    CfgData *pCfg = CfgData::instance();
    BossInfo *pBossInfo = CfgData::GetBossInfo(pCfg, BossId);
    if (!pBossInfo)
        return Pos;

    if (pBossInfo->m_vRevivePos.empty())
        return Pos;

    if (pBossInfo->m_vRevivePos.size() == 1)
        return pBossInfo->m_vRevivePos[0];

    std::vector<Position> PosVector = pBossInfo->m_vRevivePos;
    std::random_shuffle(PosVector.begin(), PosVector.end());
    if (!PosVector.empty())
        Pos = PosVector[0];
    return Pos;
}

void CWorldBoss::PacketBossInfo(Answer::NetPacket *packet, int8_t nBossType)
{
    if (!packet)
        return;

    int32_t nSize = 0;
    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    int32_t nOldOffset = packet->getWOffset();
    packet->writeInt32(0);
    packet->writeInt8(nBossType);

    for (auto &pair : m_mBossMap)
    {
        if (pair.second.nBossType == nBossType)
        {
            CfgData *pCfg = CfgData::instance();
            if (CfgData::GetBossInfo(pCfg, pair.first))
            {
                pair.second.PackageBossInfo(packet, nNowTime);
                ++nSize;
            }
        }
    }

    int32_t nNewOffset = packet->getWOffset();
    packet->setWOffset(nOldOffset);
    packet->writeInt32(nSize);
    packet->setWOffset(nNewOffset);
}

void CWorldBoss::broadcastBossKilled(int32_t Mid, std::string *p_KillerName, CharId_t KillerId)
{
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CB3);
    if (!packet)
        return;

    packet->writeInt8(0);
    packet->writeInt32(Mid);
    packet->writeUTF8(p_KillerName);
    packet->writeInt64(KillerId);
    packet->setSize(packet->getWOffset());
    GameService::broadcast(pGame, packet);
}

void CWorldBoss::broadcastBossRevive(int32_t Mid, int32_t BossId, int32_t MapId)
{
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CB3);
    if (!packet)
        return;

    packet->writeInt8(1);
    packet->writeInt32(Mid);
    packet->writeInt32(BossId);
    packet->writeInt32(MapId);
    packet->setSize(packet->getWOffset());
    GameService::broadcast(pGame, packet);
}

void CWorldBoss::GetWorldBossIcon(IconStateList *IconList)
{
    ShowIcon stu;
    stu.nId = 9;
    stu.nState = 2;
    stu.nLeftTime = -1;
    stu.nIconRight = 0;
    stu.nIconLeft = 0;
    stu.nEffects = 0;
    IconList->push_back(stu);
}

void CWorldBoss::SendWorldBossIcon(Player *pPlayer)
{
    if (!pPlayer)
        return;

    ShowIcon stu;
    stu.nId = 9;
    stu.nState = 2;
    stu.nLeftTime = -1;
    stu.nIconRight = 0;
    stu.nIconLeft = 0;
    stu.nEffects = 0;
    Player::SendIconState(pPlayer, &stu);
}

void CWorldBoss::Update(int64_t CurTick)
{
    if (CurTick - m_lastUpdateTick > 1000)
        m_lastUpdateTick = CurTick;
}

int32_t CWorldBoss::GetBossLevel(int32_t id)
{
    Answer::MutexGuard lock(&m_Lock);
    auto it = m_mBossMap.find(id);
    if (it != m_mBossMap.end())
        return it->second.nLevel;
    return 0;
}

void CWorldBoss::adjustBossAttr(CfgMonster *cfgMonster, int32_t nLevel)
{
    int32_t mid = cfgMonster->mid;
    CfgData *pCfg = CfgData::instance();
    CfgMonsterAdjust *pCfgAdjust = CfgData::GetMonsterAdjust(pCfg, mid, nLevel, 0);
    if (!pCfgAdjust)
        return;

    cfgMonster->level = pCfgAdjust->level;
    for (int32_t i = 0; i <= 49; ++i)
        cfgMonster->vAttr[i] = pCfgAdjust->vAttr[i];
}

Position CWorldBoss::OnBossRevive(int32_t nBossId, Map *pMap, Monster *pMonster)
{
    Position pos(0, 0);
    if (!pMonster)
        return pos;

    WorldBossInfo info;
    bool bFind = false;

    {
        Answer::MutexGuard lock(&m_Lock);
        auto it = m_mBossMap.find(nBossId);
        if (it != m_mBossMap.end())
        {
            if (info.nBossType == 3)
            {
                it->second.nState = 3;
                if (it->second.pPlant)
                    it->second.pPlant->remove();
            }
            else
            {
                it->second.nState = 0;
            }
            info = it->second;
            bFind = true;
        }
    }

    if (!bFind)
        return pos;

    if (info.nBossType == 1)
        broadcastBossRevive(info.nMid, info.nBossId, info.nMapId);
    if (info.nBossType == 12)
    {
        CTimer *pTimer = CTimer::instance();
        int32_t Now = CTimer::GetNow(pTimer);
        UpdateRuinsBossInfo(pMap, &info, Now);
    }
    saveBossInfo(&info);
    return GetRevivePos(nBossId);
}

void CWorldBoss::OnBossSummon(int32_t nBossId, Map *pMap, Monster *pMonster)
{
    if (!pMonster)
        return;

    WorldBossInfo info;
    bool bFind = false;

    {
        Answer::MutexGuard lock(&m_Lock);
        auto it = m_mBossMap.find(nBossId);
        if (it != m_mBossMap.end())
        {
            it->second.nState = 0;
            info = it->second;
            bFind = true;
        }
    }

    if (bFind)
    {
        broadcastBossRevive(info.nMid, info.nBossId, info.nMapId);
        saveBossInfo(&info);
    }
}

void CWorldBoss::OnBossKilled(int32_t id, int32_t nowTime, Map *pMap, Monster *pMonster, Player *pKiller)
{
    if (!pMap || !pMonster)
        return;

    WorldBossInfo info;
    KillerInfo killer;
    memset(&killer, 0, sizeof(killer));
    bool bFind = false;
    bool bLevelUp = false;

    {
        Answer::MutexGuard lock(&m_Lock);
        auto it = m_mBossMap.find(id);
        if (it == m_mBossMap.end())
        {
            WorldBossInfo::~WorldBossInfo(&info);
            return;
        }

        if (pKiller)
        {
            it->second.nState = 1;
            killer.nCharId = Player::getCid(pKiller);
            Player::getName(pKiller, &killer.strName);
            killer.nTime = nowTime;
            it->second.AddKiller(&killer);

            int32_t nLevel = it->second.nLevel;
            int32_t nMid = it->second.nMid;
            CfgData *pCfg = CfgData::instance();
            CfgMonsterAdjust *pCfgAdjust = CfgData::GetMonsterAdjust(pCfg, nMid, nLevel, 0);
            CfgMonsterAdjust *pNext = CfgData::GetMonsterAdjust(pCfg, nMid, nLevel + 1, 0);

            if (pCfgAdjust && pNext)
            {
                it->second.nExp++;
                if (it->second.nExp >= pCfgAdjust->exp)
                {
                    bLevelUp = true;
                    it->second.nLevel++;
                    it->second.nExp -= pCfgAdjust->exp;
                }
            }
        }
        else
        {
            it->second.nState = 2;
        }

        int32_t nReviveTime = GetBossRevieTime(it->second.nBossId);
        if (nReviveTime <= 0)
        {
            CfgData *pCfg = CfgData::instance();
            CfgMonster *pCfgMonster = CfgData::getMonster(pCfg, it->second.nMid);
            if (pCfgMonster)
                nReviveTime = CfgData::GetMonsterReviveTime(pCfg, pCfgMonster->revive_time, pCfgMonster->boss_sign) / 1000;
        }
        it->second.nReviveTime = nowTime + nReviveTime;
        info = it->second;
        bFind = true;
    }

    if (!bFind)
        return;

    if (bLevelUp)
    {
        CfgData *pCfg = CfgData::instance();
        CfgMonster *pCfgMonster = CfgData::getMonster(pCfg, info.nMid);
        if (pCfgMonster)
        {
            CfgMonster cfgMonster(pCfgMonster);
            adjustBossAttr(&cfgMonster, info.nLevel);
            Monster::OnLevelUp(pMonster, &cfgMonster);
        }
    }

    Monster::SetReviveTime(pMonster, info.nReviveTime);

    if (info.nBossType == 3 && pKiller)
    {
        std::string p_KillerName;
        Player::getName(pKiller, &p_KillerName);
        CharId_t Cid = Player::getCid(pKiller);
        broadcastBossKilled(info.nMid, &p_KillerName, Cid);
    }

    if (info.nBossType == 1)
    {
        if (pKiller)
        {
            std::string v45;
            Player::getName(pKiller, &v45);
            CharId_t v28 = Player::getCid(pKiller);
            broadcastBossKilled(info.nMid, &v45, v28);
        }
    }
    else if (info.nBossType == 12)
    {
        CTimer *pTimer = CTimer::instance();
        int32_t Now = CTimer::GetNow(pTimer);
        UpdateRuinsBossInfo(pMap, &info, Now);
    }

    saveBossInfo(&info);
}

void CWorldBoss::UpdateWorldBossInfo(WorldBossInfo *info)
{
    Answer::MutexGuard lock(&m_Lock);
    auto it = m_mBossMap.find(info->nBossId);
    if (it != m_mBossMap.end())
        it->second = *info;
}

void CWorldBoss::saveBossInfo(WorldBossInfo *info)
{
    DBService *pDB = Answer::Singleton<DBService>::instance();
    DBService::SaveWorldBossInfo(pDB, info);
}

void CWorldBoss::InitDropRecord()
{
    Answer::DBPool *pDB = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db(pDB);

    int32_t nSize = 0;
    Answer::MySqlQuery result = db.query("SELECT * FROM `mem_drop_record` WHERE `special`=1 ORDER BY `time` DESC LIMIT 5");
    while (!result.eof())
    {
        DropRecord record(result.getStringValue("name", ""), 0, 0, 0, 0, 0, 0);
        record.strName = result.getStringValue("name", "");
        record.nCharId = result.getInt64Value("cid", 0);
        record.nMapId = result.getIntValue("mapid", 0);
        record.nMid = result.getIntValue("mid", 0);
        record.nRecord = result.getIntValue("record", 0);
        record.nTime = result.getIntValue("time", 0);
        record.nSpecial = result.getIntValue("special", 0);
        m_dropRecordsSpecial.push_front(record);
        if (++nSize > 4)
            break;
        result.nextRow();
    }

    result = db.query("SELECT * FROM `mem_drop_record` WHERE `special`=0 ORDER BY `time` DESC LIMIT 30");
    while (!result.eof())
    {
        DropRecord record(result.getStringValue("name", ""), 0, 0, 0, 0, 0, 0);
        record.strName = result.getStringValue("name", "");
        record.nCharId = result.getInt64Value("cid", 0);
        record.nMapId = result.getIntValue("mapid", 0);
        record.nMid = result.getIntValue("mid", 0);
        record.nRecord = result.getIntValue("record", 0);
        record.nTime = result.getIntValue("time", 0);
        record.nSpecial = result.getIntValue("special", 0);
        m_dropRecords.push_front(record);
        if (++nSize > 29)
            break;
        result.nextRow();
    }
}

void CWorldBoss::SendDropRecord(int8_t connid, int16_t nGateIndex)
{
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, connid, Answer::PackType::PACK_DISPATCH, 0x2ED6);
    if (!packet)
        return;

    {
        Answer::MutexGuard lock(&m_RecordLock);
        int16_t nTotalSize = (int16_t)(m_dropRecords.size() + m_dropRecordsSpecial.size());
        packet->writeInt16(nTotalSize);

        for (auto &rec : m_dropRecordsSpecial)
        {
            packet->writeInt32(rec.nRecord);
            packet->writeUTF8(&rec.strName);
            packet->writeInt64(rec.nCharId);
            packet->writeInt32(rec.nMapId);
            packet->writeInt32(rec.nMid);
            packet->writeInt32(rec.nTime);
        }
        for (auto &rec : m_dropRecords)
        {
            packet->writeInt32(rec.nRecord);
            packet->writeUTF8(&rec.strName);
            packet->writeInt64(rec.nCharId);
            packet->writeInt32(rec.nMapId);
            packet->writeInt32(rec.nMid);
            packet->writeInt32(rec.nTime);
        }
    }

    packet->setSize(packet->getWOffset());
    GameService::sendPacketTo(pGame, connid, nGateIndex, packet);
}

int32_t CWorldBoss::GetBossRevie(int32_t mId)
{
    int32_t ReviceTime = 0;
    CTimer *pTimer = CTimer::instance();
    int32_t NowTime = CTimer::GetNow(pTimer);
    tm NowLocal;
    CTimer::GetLocalNow(&NowLocal, pTimer);

    auto it = m_mBossMap.find(mId);
    if (it != m_mBossMap.end())
    {
        if (it->second.nState == 0)
            return 0;
        return it->second.nReviveTime - NowTime;
    }
    return ReviceTime;
}

void CWorldBoss::SendRuinsBossInfo(Player *player, BossType nBossType, int32_t nMapId)
{
    if (!player)
        return;

    int8_t ConnId = Player::getConnId(player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CDD);
    if (!packet)
        return;

    int8_t nSize = 0;
    CTimer *pTimer = CTimer::instance();
    int32_t NowTime = CTimer::GetNow(pTimer);
    int32_t OldOffset = packet->getWOffset();
    packet->writeInt8(0);

    for (auto &pair : m_mBossMap)
    {
        if (pair.second.nBossType != (int32_t)nBossType || pair.second.nMapId != nMapId)
            continue;

        packet->writeInt32(pair.second.nBossId);
        packet->writeInt32(pair.second.nMid);
        packet->writeInt32(pair.second.nMapId);

        int32_t v12;
        if (pair.second.nState)
            v12 = pair.second.nReviveTime - NowTime;
        else
            v12 = 0;
        packet->writeInt32(v12);
        ++nSize;
    }

    int32_t NewOffset = packet->getWOffset();
    packet->setWOffset(OldOffset);
    packet->writeInt8(nSize);
    packet->setWOffset(NewOffset);
    packet->setSize(packet->getWOffset());

    int16_t GateIndex = Player::getGateIndex(player);
    int8_t v15 = Player::getConnId(player);
    GameService::sendPacketTo(pGame, v15, GateIndex, packet);
}

void CWorldBoss::UpdateRuinsBossInfo(Map *pMap, const WorldBossInfo *info, int32_t nNowTime)
{
    if (!pMap)
        return;

    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CDE);
    if (!packet)
        return;

    packet->writeInt32(info->nBossId);
    packet->writeInt32(info->nMid);
    packet->writeInt32(info->nMapId);

    int32_t v5;
    if (info->nState)
        v5 = info->nReviveTime - nNowTime;
    else
        v5 = 0;
    packet->writeInt32(v5);
    packet->setSize(packet->getWOffset());
    Map::broadcast(pMap, packet);
}
