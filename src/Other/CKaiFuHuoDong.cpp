#include "common.h"
#include "Other/CKaiFuHuoDong.h"

#include <cstring>
#include <algorithm>

CKaiFuHuoDong::CKaiFuHuoDong()
{
}

CKaiFuHuoDong::~CKaiFuHuoDong()
{
}

void CKaiFuHuoDong::Init(int32_t line)
{
    CfgData *pCfg = CfgData::instance();
    m_nHDLastDay = CfgData::getKaiFuHuoDongLastDay(pCfg);

    const CfgKaiFuHuoDongTable *pTable = CfgData::GetKaiFuHuoDongTable(pCfg);
    if (pTable)
    {
        for (auto &pair : pTable->m_KaiFuHuoDongData)
            m_KaiFuHuoDongCfg[pair.first] = pair.second;
    }

    const CfgFlopTable *pFlop = CfgData::GetFlopTable(pCfg, 1);
    if (pFlop)
    {
        m_FlopFreeTimes = pFlop->nFreeTimes;
        m_FlopBuyTimes = pFlop->nBuyTimes;
        m_FlopStartTime = pFlop->nStartTime;
        m_FlopEndTime = pFlop->nEndTime;
        for (auto &cost : pFlop->vCost)
            m_FlopCost.push_back(cost);
        for (auto &type : pFlop->vType)
            m_FlopType.push_back(type);
        m_FlopSize = (int32_t)m_FlopType.size();
    }

    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `mem_boss_first_killed`");
    Answer::MySqlQuery *pQuery = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result(pQuery);
    while (!result.eof())
    {
        BossFirstKilled stu;
        stu.nBossId = result.getIntValue("id", 0);
        stu.Mid = result.getIntValue("mid", 0);
        stu.Cid = result.getInt64Value("cid", 0);
        stu.nName = result.getStringValue("name", "");
        stu.nTime = result.getIntValue("time", 0);
        m_BossFirstKilledMap[stu.nBossId] = stu;
        result.nextRow();
    }
}

CfgKaiFuHuoDongData *CKaiFuHuoDong::GetKaiFuHuoDongData(int32_t Index)
{
    auto it = m_KaiFuHuoDongCfg.find(Index);
    if (it != m_KaiFuHuoDongCfg.end())
        return &it->second;
    return nullptr;
}

bool CKaiFuHuoDong::InTime()
{
    CfgData *pCfg = CfgData::instance();
    return CfgData::getServerType(pCfg) != 1 && GetDaysFromServerStart() <= m_nHDLastDay;
}

int32_t CKaiFuHuoDong::OnGetReward(Player *pPlayer, Answer::NetPacket *inPacket)
{
    if (!pPlayer || !inPacket)
        return 2;

    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return 10002;

    if (!InTime())
        return 10002;

    int32_t nDaysFromServerStart = GetDaysFromServerStart();
    int32_t nIndex = inPacket->readInt32();

    if (nIndex <= 0)
        return 10002;

    CfgKaiFuHuoDongData *pCfgData = GetKaiFuHuoDongData(nIndex);
    if (!pCfgData)
        return 10002;

    if (IsAlreadyGet(pPlayer, nIndex))
        return 10002;

    if (Player::getLevel(pPlayer) < pCfgData->MinLv)
        return 10002;

    CExtCharBag *pBag = Player::GetBag(pPlayer);
    if (CExtCharBag::GetFreeSlotCount(pBag) <= 0)
        return 30011;

    int32_t nLimit = GetLimitCount(nIndex);
    if (nLimit >= pCfgData->Limit)
        return 10002;

    int32_t nCount = pCfgData->Count;
    if (nCount > 0)
    {
        MemChrBagVector items;
        MemChrBag item;
        memset(&item, 0, sizeof(item));
        item.itemClass = pCfgData->Item.itemClass;
        item.itemId = pCfgData->Item.itemId;
        item.itemCount = pCfgData->Item.itemCount * nCount;
        items.push_back(item);
        if (!CExtCharBag::AddItem(pBag, &items, ITEM_CHANGE_REASON::ICR_KAIFUHUODONG_GET))
            return 10002;
    }

    int32_t nExp = pCfgData->Exp;
    if (nExp > 0)
        Player::AddExp(pPlayer, nExp, EXP_CHANGE_REASON::ECR_KAIFUHUODONG);

    AddRewardRecord(pPlayer, nIndex);
    if (pCfgData->GongGaoId > 0)
        GongGao(pPlayer, pCfgData->GongGaoId, nIndex);

    SendHuoDongInfo(pPlayer);
    SendKaiFuHuoDongIcon(pPlayer);
    return 0;
}

int32_t CKaiFuHuoDong::GetLimitCount(int32_t index)
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[1024];
    snprintf(szSQL, sizeof(szSQL), "SELECT `count` FROM `mem_kaifuhuodong` WHERE `index`=%d", index);
    Answer::MySqlQuery *pQuery = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result(pQuery);
    if (!result.eof())
        return result.getIntValue("count", 0);
    return 0;
}

void CKaiFuHuoDong::AddLimitCount(int32_t index, int32_t Values)
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    int32_t nOld = GetLimitCount(index);
    int32_t nNew = nOld + Values;
    char szSQL[1024];
    snprintf(szSQL, sizeof(szSQL), "REPLACE INTO `mem_kaifuhuodong` (`index`,`count`) VALUES (%d,%d)", index, nNew);
    Answer::MySqlDBGuard::query(&db, szSQL);
}

void CKaiFuHuoDong::GongGao(Player *pPlayer, int32_t GongGaoId, int32_t Index)
{
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, Player::getConnId(pPlayer), Answer::PackType::PACK_DISPATCH, 0x2CB3);
    if (!packet)
        return;

    packet->writeInt16(Player::getGateIndex(pPlayer));
    packet->writeInt64(Player::getCid(pPlayer));
    std::string val;
    Player::getName(pPlayer, &val);
    packet->writeUTF8(&val);
    packet->writeInt32(GongGaoId);
    packet->writeInt32(Index);
    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, Player::getConnId(pPlayer), packet);
}

void CKaiFuHuoDong::AddRewardRecord(Player *pPlayer, int32_t Index)
{
    if (!pPlayer)
        return;

    if (Index > 32)
    {
        int32_t nRecord = Player::getRecord(pPlayer, 1081);
        Player::updateRecord(pPlayer, 1081, nRecord | (1 << (Index - 33)));
    }
    else
    {
        int32_t nRecord = Player::getRecord(pPlayer, 1032);
        Player::updateRecord(pPlayer, 1032, nRecord | (1 << (Index - 1)));
    }
}

bool CKaiFuHuoDong::IsAlreadyGet(Player *pPlayer, int32_t Index)
{
    if (!pPlayer)
        return true;

    if (Index > 32)
    {
        if ((Player::getRecord(pPlayer, 1081) >> (Index - 33) & 1) != 1)
            return false;
    }
    else
    {
        if ((Player::getRecord(pPlayer, 1032) >> (Index - 1) & 1) != 1)
            return false;
    }
    return true;
}

void CKaiFuHuoDong::SendHuoDongInfo(Player *pPlayer)
{
    if (!pPlayer)
        return;

    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, Player::getConnId(pPlayer), Answer::PackType::PACK_DISPATCH, 0x2CB2);
    if (!packet)
        return;

    int32_t nDays = GetDaysFromServerStart();
    packet->writeInt32(m_nHDLastDay);
    packet->writeInt32(nDays);
    packet->writeInt32(0);

    int32_t nCount = 0;
    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.Day > nDays)
            break;
        if (pair.second.Day == nDays || pair.second.Day == -1)
            nCount++;
    }
    packet->writeInt32(nCount);

    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.Day > nDays)
            break;
        if (pair.second.Day != nDays && pair.second.Day != -1)
            continue;

        packet->writeInt32(pair.first);
        packet->writeInt32(pair.second.Day);
        int8_t nState = 0;
        if (IsAlreadyGet(pPlayer, pair.first))
            nState = 2;
        else if (Player::getLevel(pPlayer) >= pair.second.MinLv)
            nState = 1;
        packet->writeInt8(nState);

        if (pair.second.Limit > 0)
            packet->writeInt32(pair.second.Limit - GetLimitCount(pair.first));
        else
            packet->writeInt32(0);
    }

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, Player::getConnId(pPlayer), packet);
}

int32_t CKaiFuHuoDong::AskInfo(Player *pPlayer, Answer::NetPacket *inPacket)
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return 10002;

    if (!pPlayer || !inPacket)
        return 2;

    SendHuoDongInfo(pPlayer);
    return 0;
}

int32_t CKaiFuHuoDong::GetDaysFromServerStart()
{
    CfgData *pCfg = CfgData::instance();
    if (CfgData::isUniteServer(pCfg))
        return 99999;

    return CfgData::getServerDiffDay(CfgData::instance(), SERVER_TYPE::SVT_NORMAL);
}

void CKaiFuHuoDong::SendKaiFuHuoDongIcon(Player *pPlayer)
{
    if (!pPlayer)
        return;

    CfgData *pCfg = CfgData::instance();
    if (CfgData::isUniteServer(pCfg))
        return;

    CFunctionOpen *pFunc = Player::GetFunctionOpen(pPlayer);
    if (!CFunctionOpen::isFuncOpen(pFunc, 405))
        return;

    ShowIcon stu;
    memset(&stu, 0, sizeof(stu));
    stu.nId = 0x20000004;
    stu.nLeftTime = -1;

    int32_t nDays = GetDaysFromServerStart();
    if (nDays <= m_nHDLastDay)
        stu.nState = HaveRewardCount(pPlayer) > 0 ? 2 : 1;
    else
        stu.nState = 4;

    Player::SendIconState(pPlayer, &stu);
}

int32_t CKaiFuHuoDong::HaveRewardCount(Player *pPlayer)
{
    if (!pPlayer)
        return 0;

    int32_t nCount = 0;
    int32_t nDays = GetDaysFromServerStart();

    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.Day > nDays)
            break;

        if (IsAlreadyGet(pPlayer, pair.first))
            continue;
        if (Player::getLevel(pPlayer) < pair.second.MinLv)
            continue;

        int32_t nLimit = GetLimitCount(pair.first);
        if (nLimit < pair.second.Limit)
            nCount++;
    }
    return nCount;
}

void CKaiFuHuoDong::OnKilledMonster(Player *pPlayer, int32_t Mid)
{
    if (!pPlayer)
        return;

    int32_t nDays = GetDaysFromServerStart();
    if (nDays > m_nHDLastDay)
        return;

    for (auto &pair : m_BossFirstKilledMap)
    {
        if (pair.second.Mid == Mid && pair.second.Cid <= 0)
        {
            pair.second.Cid = Player::getCid(pPlayer);
            pair.second.nName = Player::getName(pPlayer);
            pair.second.nTime = CTimer::GetNow(CTimer::instance());

            Answer::DBPool *pDB = Answer::DBPool::instance();
            Answer::MySqlDBGuard db(pDB);
            char szSQL[4096];
            snprintf(szSQL, sizeof(szSQL),
                "INSERT INTO `mem_boss_first_killed` (`id`, `mid`, `cid`, `name`, `time`) VALUES (%d, %d, %lld, '%s', %d) "
                "ON DUPLICATE KEY UPDATE `cid`=%lld, `name`='%s', `time`=%d",
                pair.second.nBossId, Mid, pair.second.Cid, pair.second.nName.c_str(), pair.second.nTime,
                pair.second.Cid, pair.second.nName.c_str(), pair.second.nTime);
            Answer::MySqlDBGuard::query(&db, szSQL);

            SendBossFirstKillInfo(pPlayer);
            break;
        }
    }
}

void CKaiFuHuoDong::SendBossFirstKillInfo(Player *pPlayer)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2CB4);
    if (!packet)
        return;

    int32_t nCount = 0;
    for (auto &pair : m_BossFirstKilledMap)
    {
        if (pair.second.Cid > 0)
            nCount++;
    }
    packet->writeInt32(nCount);
    for (auto &pair : m_BossFirstKilledMap)
    {
        if (pair.second.Cid > 0)
        {
            packet->writeInt32(pair.second.nBossId);
            packet->writeInt32(pair.second.Mid);
            packet->writeInt64(pair.second.Cid);
            packet->writeUTF8(&pair.second.nName);
            packet->writeInt32(pair.second.nTime);
        }
    }

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, nConnId, packet);
}

bool CKaiFuHuoDong::HaveTeHuiGift(Player *pPlayer)
{
    if (!pPlayer)
        return false;

    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.TeHui > 0 && pair.second.Day <= GetDaysFromServerStart())
        {
            if (!IsAlreadyGet(pPlayer, pair.first))
                return true;
        }
    }
    return false;
}

bool CKaiFuHuoDong::HaveBuyPetItem(Player *pPlayer)
{
    if (!pPlayer)
        return false;

    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.PetItem > 0 && pair.second.Day <= GetDaysFromServerStart())
        {
            if (!IsAlreadyGet(pPlayer, pair.first))
                return true;
        }
    }
    return false;
}

void CKaiFuHuoDong::GetKaiFuPetIcon(Player *pPlayer, IconStateList *IconList)
{
    if (!pPlayer)
        return;

    CfgData *pCfg = CfgData::instance();
    if (CfgData::isUniteServer(pCfg))
        return;

    CFunctionOpen *pFunc = Player::GetFunctionOpen(pPlayer);
    if (!CFunctionOpen::isFuncOpen(pFunc, 405))
        return;

    if (HaveBuyPetItem(pPlayer))
    {
        ShowIcon stu;
        memset(&stu, 0, sizeof(stu));
        stu.nId = 0x2000000B;
        stu.nLeftTime = -1;
        stu.nState = 2;
        IconList->push_back(stu);
    }
}

void CKaiFuHuoDong::SendKaiFuPetIcon(Player *pPlayer)
{
    if (!pPlayer)
        return;

    CfgData *pCfg = CfgData::instance();
    if (CfgData::isUniteServer(pCfg))
        return;

    CFunctionOpen *pFunc = Player::GetFunctionOpen(pPlayer);
    if (!CFunctionOpen::isFuncOpen(pFunc, 405))
        return;

    if (HaveBuyPetItem(pPlayer))
    {
        ShowIcon stu;
        memset(&stu, 0, sizeof(stu));
        stu.nId = 0x2000000B;
        stu.nLeftTime = -1;
        stu.nState = 2;
        Player::SendIconState(pPlayer, &stu);
    }
}

int32_t CKaiFuHuoDong::GetKaiFuPetStartDay()
{
    int32_t nMinDay = 9999;
    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.PetItem > 0 && pair.second.Day >= 0 && pair.second.Day < nMinDay)
            nMinDay = pair.second.Day;
    }
    return nMinDay == 9999 ? 0 : nMinDay;
}

int32_t CKaiFuHuoDong::GetKaiFuPetEndDay()
{
    int32_t nMaxDay = 0;
    for (auto &pair : m_KaiFuHuoDongCfg)
    {
        if (pair.second.PetItem > 0 && pair.second.Day > nMaxDay)
            nMaxDay = pair.second.Day;
    }
    return nMaxDay;
}

int32_t CKaiFuHuoDong::GetFlopType(int32_t nFlopId)
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return 0;

    if (nFlopId >= 0 && nFlopId < m_FlopSize)
        return m_FlopType[nFlopId];
    return 0;
}

bool CKaiFuHuoDong::CanFlopDraw(Player *pPlayer, int32_t FlopId)
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return false;

    if (!pPlayer || !InTime())
        return false;

    CExtOperateLimit *pLimit = Player::GetOperateLimit(pPlayer);
    int32_t nFlopTimes = CExtOperateLimit::GetLimitCount(pLimit, 1076);
    if (nFlopTimes < (int32_t)m_FlopFreeTimes)
        return true;

    int32_t nBuyTimes = CExtOperateLimit::GetLimitCount(pLimit, 1077);
    if (nBuyTimes < (int32_t)m_FlopBuyTimes)
    {
        if (FlopId >= 0 && FlopId < (int32_t)m_FlopCost.size())
        {
            ItemData *pCost = &m_FlopCost[FlopId];
            CExtCharBag *pBag = Player::GetBag(pPlayer);
            if (CExtCharBag::GetItemCount(pBag, pCost->m_nItemClass, pCost->m_nItemId) >= pCost->m_nCount)
                return true;
        }
    }
    return false;
}

int32_t CKaiFuHuoDong::GetMaxFlopTimes(Player *pPlayer)
{
    if (!pPlayer)
        return 0;

    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return 0;

    CExtOperateLimit *pLimit = Player::GetOperateLimit(pPlayer);
    int32_t nFlopTimes = CExtOperateLimit::GetLimitCount(pLimit, 1076);
    if (nFlopTimes < (int32_t)m_FlopFreeTimes)
        return (int32_t)m_FlopFreeTimes;

    int32_t nBuyTimes = CExtOperateLimit::GetLimitCount(pLimit, 1077);
    if (nBuyTimes < (int32_t)m_FlopBuyTimes)
    {
        int32_t nTimes = (int32_t)(m_FlopFreeTimes + m_FlopBuyTimes - nBuyTimes);
        int32_t nValueCount = 0;
        for (size_t i = 0; i < m_FlopType.size(); i++)
        {
            if (m_FlopType[i] > 0)
                nValueCount++;
        }
        if (nTimes > nValueCount)
            nTimes = nValueCount;
        return nTimes;
    }
    return 0;
}
