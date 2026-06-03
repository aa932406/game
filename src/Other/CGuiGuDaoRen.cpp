#include "common.h"
#include "Other/CGuiGuDaoRen.h"

#include <cstring>

CGuiGuDaoRen::CGuiGuDaoRen()
{
}

CGuiGuDaoRen::~CGuiGuDaoRen()
{
}

void CGuiGuDaoRen::Init(int32_t line)
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[4096];

    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `mem_back_item_count`");
    Answer::MySqlQuery *pQuery = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result(pQuery);
    while (!result.eof())
    {
        int32_t nNpcId = result.getIntValue("npc_id", 0);
        int32_t nCount = result.getIntValue("count", 0);
        BackItemCount[nNpcId] = nCount;
        result.nextRow();
    }

    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "SELECT value FROM `mem_global` WHERE `key`='back_equip_count'");
    Answer::MySqlQuery *pQuery2 = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result2(pQuery2);
    if (!result2.eof())
    {
        EquipCount = result2.getIntValue("value", 0);
    }

    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `mem_back_equip_rank` ORDER BY `score` DESC LIMIT 30");
    Answer::MySqlQuery *pQuery3 = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result3(pQuery3);
    while (!result3.eof())
    {
        EquipBackRankCfg stu;
        stu.nRank = result3.getIntValue("rank", 0);
        stu.nCid = result3.getInt64Value("cid", 0);
        stu.nName = result3.getStringValue("name", "");
        stu.nScore = result3.getIntValue("score", 0);
        m_EquipBackRank.push_back(stu);
        result3.nextRow();
    }
}

int32_t CGuiGuDaoRen::OnAskBackItemCount(Player *pPlayer, Answer::NetPacket *packet)
{
    if (!packet || !pPlayer)
        return 10002;

    int32_t nNpcId = packet->readInt32();
    SendBackItemCount(pPlayer, nNpcId);
    return 0;
}

int32_t CGuiGuDaoRen::OnBackItem(Player *pPlayer, Answer::NetPacket *packet)
{
    if (!packet || !pPlayer)
        return 2;

    CfgData *pCfg = CfgData::instance();
    int32_t nNpcId = packet->readInt32();
    int32_t nItemId = packet->readInt32();
    int32_t nCount = packet->readInt32();

    CExtCharBag *pBag = Player::GetBag(pPlayer);
    ItemData itemData;
    memset(&itemData, 0, sizeof(itemData));
    itemData.m_nItemClass = 2;
    itemData.m_nItemId = nItemId;
    itemData.m_nCount = nCount;

    if (CExtCharBag::GetItemCount(pBag, 2, nItemId) < nCount)
        return 10002;

    ItemDataList itemList;
    itemList.push_back(itemData);
    if (!CExtCharBag::RemoveItem(pBag, &itemList, ITEM_CHANGE_REASON::ICR_GUIGUDAOREN_BACK))
        return 10002;

    int32_t nGold = CfgData::getBackItemGold(pCfg, nItemId) * nCount;
    Player::AddCurrency(pPlayer, CURRENCY_GOLD, nGold, CURRENCY_CHANGE_REASON::GCR_GUIGUDAOREN_BACK, 0);

    auto it = BackItemCount.find(nNpcId);
    if (it != BackItemCount.end())
        it->second += nCount;
    else
        BackItemCount[nNpcId] = nCount;

    UpdateBackItemCount(nNpcId);
    SendBackItemCount(pPlayer, nNpcId);
    return 0;
}

void CGuiGuDaoRen::SendBackItemCount(Player *pPlayer, int32_t NpcId)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2B79);
    if (!packet)
        return;

    packet->writeInt32(NpcId);
    auto it = BackItemCount.find(NpcId);
    if (it != BackItemCount.end())
        packet->writeInt32(it->second);
    else
        packet->writeInt32(0);

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, nConnId, packet);
}

void CGuiGuDaoRen::UpdateBackItemCount(int32_t NpcId)
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    char szSQL[1024];
    auto it = BackItemCount.find(NpcId);
    int32_t nCount = (it != BackItemCount.end()) ? it->second : 0;
    snprintf(szSQL, sizeof(szSQL), "REPLACE INTO `mem_back_item_count` (`npc_id`, `count`) VALUES (%d, %d)", NpcId, nCount);
    Answer::MySqlDBGuard::query(pDB, szSQL);
}

int32_t CGuiGuDaoRen::OnAskBackEquipCount(Player *pPlayer, Answer::NetPacket *packet)
{
    SendBackEquipCount(pPlayer);
    return 0;
}

int32_t CGuiGuDaoRen::OnBackEquipCount(Player *pPlayer, Answer::NetPacket *InPacket)
{
    if (!InPacket || !pPlayer)
        return 2;

    int32_t nIndex = InPacket->readInt32();
    CfgData *pCfg = CfgData::instance();

    CExtCharBag *pBag = Player::GetBag(pPlayer);
    ItemData itemData;
    memset(&itemData, 0, sizeof(itemData));
    itemData.m_nItemClass = 2;
    itemData.m_nItemId = nIndex;
    itemData.m_nCount = 1;

    if (CExtCharBag::GetItemCount(pBag, 2, nIndex) < 1)
        return 10002;

    const CfgEquipTable *pEquip = CfgData::GetEquipTable(pCfg, nIndex);
    if (pEquip)
    {
        int32_t nQuality = pEquip->nQuality;
        if (nQuality >= 5)
        {
            ItemDataList itemList;
            itemList.push_back(itemData);
            if (CExtCharBag::RemoveItem(pBag, &itemList, ITEM_CHANGE_REASON::ICR_GUIGUDAOREN_BACK))
            {
                EquipCount++;
                UpdateEquipCount();
                SendBackEquipCount(pPlayer);
            }
        }
    }
    return 0;
}

void CGuiGuDaoRen::SendBackEquipCount(Player *pPlayer)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2B7B);
    if (!packet)
        return;

    int32_t nMaxCount = GetMaxCount();
    packet->writeInt32(nMaxCount);
    packet->writeInt32(EquipCount);

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, nConnId, packet);
}

void CGuiGuDaoRen::UpdateEquipCount()
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    char szSQL[1024];
    snprintf(szSQL, sizeof(szSQL), "REPLACE INTO `mem_global` (`key`, `value`) VALUES ('back_equip_count', %d)", EquipCount);
    Answer::MySqlDBGuard::query(pDB, szSQL);
}

int32_t CGuiGuDaoRen::GetMaxCount()
{
    CfgData *pCfg = CfgData::instance();
    int32_t nCount = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL) + 1;
    if (nCount <= 44)
        return 0;
    if (nCount <= 59)
        return (nCount - 44) * 3;
    return 45;
}

void CGuiGuDaoRen::OnNewMinute(int32_t nMinute)
{
    if (nMinute != 0)
        return;

    int32_t nEquipCount = EquipCount;
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[4096];

    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `mem_back_equip_rank` ORDER BY `score` DESC LIMIT 30");
    Answer::MySqlQuery *pQuery = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result(pQuery);

    int32_t nIndex = 0;
    while (!result.eof())
    {
        if (nIndex >= 30)
            break;

        EquipBackRankCfg stu;
        stu.nRank = ++nIndex;
        stu.nCid = result.getInt64Value("cid", 0);
        stu.nName = result.getStringValue("name", "");
        stu.nScore = result.getIntValue("score", 0);
        UpdateRankDate(&stu);
        result.nextRow();
    }
}

int32_t CGuiGuDaoRen::OnAskEquipBackRank(Player *pPlayer, Answer::NetPacket *packet)
{
    SendBackEquipRank(pPlayer);
    return 0;
}

void CGuiGuDaoRen::SendBackEquipRank(Player *pPlayer)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2B7C);
    if (!packet)
        return;

    packet->writeInt32((int32_t)m_EquipBackRank.size());
    for (auto &rank : m_EquipBackRank)
    {
        packet->writeInt32(rank.nRank);
        packet->writeInt64(rank.nCid);
        packet->writeUTF8(&rank.nName);
        packet->writeInt32(rank.nScore);
    }

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, nConnId, packet);
}

void CGuiGuDaoRen::UpdateRankDate(EquipBackRankCfg *p_Stu)
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    char szSQL[4096];
    snprintf(szSQL, sizeof(szSQL),
        "REPLACE INTO `mem_back_equip_rank` (`rank`, `cid`, `name`, `score`) VALUES (%d, %lld, '%s', %d)",
        p_Stu->nRank, p_Stu->nCid, p_Stu->nName.c_str(), p_Stu->nScore);
    Answer::MySqlDBGuard::query(pDB, szSQL);
}

void CGuiGuDaoRen::AddRongHeRecord(RongHeRecord *p_Stu)
{
    if (m_RongHeRecordList.size() > 19)
        m_RongHeRecordList.pop_front();

    m_RongHeRecordList.push_back(*p_Stu);
    SendOneRongHeRecord(p_Stu);
}

void CGuiGuDaoRen::SendAllRongHeRecord(Player *pPlayer)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = Player::getConnId(pPlayer);
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2871);
    if (!packet)
        return;

    packet->writeInt32((int32_t)m_RongHeRecordList.size());
    for (auto &record : m_RongHeRecordList)
    {
        packet->writeInt32(record.nTime);
        packet->writeInt32(record.nCostId);
        packet->writeInt32(record.nCostCount);
        packet->writeInt32(record.nGiveId);
        packet->writeInt32(record.nGiveCount);
        packet->writeInt32(record.nSuccess);
    }

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, nConnId, packet);
}

void CGuiGuDaoRen::SendOneRongHeRecord(RongHeRecord *p_Stu)
{
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, 0, Answer::PackType::PACK_DISPATCH, 0x2872);
    if (!packet)
        return;

    packet->writeInt32(p_Stu->nTime);
    packet->writeInt32(p_Stu->nCostId);
    packet->writeInt32(p_Stu->nCostCount);
    packet->writeInt32(p_Stu->nGiveId);
    packet->writeInt32(p_Stu->nGiveCount);
    packet->writeInt32(p_Stu->nSuccess);

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, 0, packet);
}

void CGuiGuDaoRen::AddRongHeCount(int32_t nCostCount, int32_t nGetCount)
{
    m_CostCount += nCostCount;
    m_GetCount += nGetCount;
    if (m_CostCount > 5000000)
    {
        m_CostCount = 0;
        m_GetCount = 0;
    }
}

void CGuiGuDaoRen::UpdateRongHeCount()
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[1024];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "REPLACE INTO `mem_global` (`key`, `value`) VALUES ('ronghe_cost_count', %d)", m_CostCount);
    Answer::MySqlDBGuard::query(&db, szSQL);
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "REPLACE INTO `mem_global` (`key`, `value`) VALUES ('ronghe_get_count', %d)", m_GetCount);
    Answer::MySqlDBGuard::query(&db, szSQL);
}

void CGuiGuDaoRen::SaveRecordToDB()
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    for (auto &record : m_RongHeRecordList)
    {
        char szSQL[1024];
        snprintf(szSQL, sizeof(szSQL),
            "INSERT INTO `mem_ronghe_record` (`time`, `cost_id`, `cost_count`, `give_id`, `give_count`, `success`) VALUES (%d, %d, %d, %d, %d, %d)",
            record.nTime, record.nCostId, record.nCostCount, record.nGiveId, record.nGiveCount, record.nSuccess);
        Answer::MySqlDBGuard::query(&db, szSQL);
    }
}
