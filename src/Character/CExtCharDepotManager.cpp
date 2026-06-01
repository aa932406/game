// CExtCharDepotManager.cpp — 仓库管理器完整实现
#include "Character/CExtCharDepotManager.h"

// ============================================================================
// 单例
// ============================================================================
CExtCharDepotManager* CExtCharDepotManager::GetInstance()
{
    return Answer::Singleton<CExtCharDepotManager>::instance();
}

// ============================================================================
// 生命周期
// ============================================================================
CExtCharDepotManager::CExtCharDepotManager()
{
}

CExtCharDepotManager::~CExtCharDepotManager()
{
    // 清理所有仓库实例
    for (auto& kv : m_mDepots)
    {
        if (kv.second)
        {
            kv.second->OnCleanUp();
            delete kv.second;
        }
    }
    m_mDepots.clear();
}

// ============================================================================
// 仓库实例管理
// ============================================================================
CExtCharDepot* CExtCharDepotManager::CreateDepot(Player* pPlayer)
{
    if (!pPlayer) return nullptr;

    CExtCharDepot* pDepot = new CExtCharDepot();
    pDepot->Init(pPlayer);
    return pDepot;
}

void CExtCharDepotManager::DestroyDepot(Player* pPlayer)
{
    if (!pPlayer) return;

    int32_t nPlayerId = Player::getCid(pPlayer);
    {
        Answer::RwLock::WriteLock lock(&m_rwLock);
        auto it = m_mDepots.find(nPlayerId);
        if (it != m_mDepots.end())
        {
            CExtCharDepot* pDepot = it->second;
            if (pDepot)
            {
                pDepot->OnCleanUp();
                delete pDepot;
            }
            m_mDepots.erase(it);
        }
    }
}

CExtCharDepot* CExtCharDepotManager::GetDepot(Player* pPlayer)
{
    if (!pPlayer) return nullptr;

    int32_t nPlayerId = Player::getCid(pPlayer);
    {
        Answer::RwLock::WriteLock lock(&m_rwLock);
        auto it = m_mDepots.find(nPlayerId);
        if (it != m_mDepots.end())
            return it->second;

        // 首次访问，创建新实例
        CExtCharDepot* pDepot = CreateDepot(pPlayer);
        if (pDepot)
            m_mDepots[nPlayerId] = pDepot;
        return pDepot;
    }
}

void CExtCharDepotManager::OnPlayerLogin(Player* pPlayer)
{
    if (!pPlayer) return;

    // 注意: 调用方需确保 OnLoadFromDB 在 OnPlayerLogin 之前调用
    // 否则发送的数据会是空的

    // 确保仓库实例已创建
    GetDepot(pPlayer);

    // 发送仓库信息
    CExtCharDepot* pDepot = GetDepot(pPlayer);
    if (pDepot)
    {
        pDepot->SendDepotInfo();
        pDepot->SendDepotItem();
        pDepot->SendDepotCurrency();
    }
}

void CExtCharDepotManager::OnPlayerLogout(Player* pPlayer)
{
    if (!pPlayer) return;

    // 先保存后销毁
    int32_t nPlayerId = Player::getCid(pPlayer);
    {
        Answer::RwLock::WriteLock lock(&m_rwLock);
        auto it = m_mDepots.find(nPlayerId);
        if (it != m_mDepots.end())
        {
            CExtCharDepot* pDepot = it->second;
            if (pDepot)
            {
                pDepot->OnCleanUp();
                delete pDepot;
            }
            m_mDepots.erase(it);
        }
    }
}

// ============================================================================
// 数据库操作
// ============================================================================
void CExtCharDepotManager::OnLoadFromDB(Player* pPlayer, const PlayerDBData* dbData)
{
    if (!pPlayer || !dbData) return;

    CExtCharDepot* pDepot = GetDepot(pPlayer);
    if (pDepot)
        pDepot->OnLoadFromDB(dbData);
}

void CExtCharDepotManager::OnSaveToDB(Player* pPlayer, PlayerDBData* dbData)
{
    if (!pPlayer || !dbData) return;

    int32_t nPlayerId = Player::getCid(pPlayer);
    {
        Answer::RwLock::ReadLock lock(&m_rwLock);
        auto it = m_mDepots.find(nPlayerId);
        if (it != m_mDepots.end() && it->second)
            it->second->OnSaveToDB(dbData);
    }
}

// ============================================================================
// 全局更新
// ============================================================================
void CExtCharDepotManager::OnUpdate(int64_t curTick)
{
    Answer::RwLock::ReadLock lock(&m_rwLock);
    for (auto& kv : m_mDepots)
    {
        if (kv.second)
            kv.second->OnUpdate(curTick);
    }
}

void CExtCharDepotManager::OnDaySwitch(int32_t nDiffDays)
{
    Answer::RwLock::ReadLock lock(&m_rwLock);
    for (auto& kv : m_mDepots)
    {
        if (kv.second)
            kv.second->OnDaySwitch(nDiffDays);
    }
}

// ============================================================================
// 跨玩家操作
// ============================================================================
bool CExtCharDepotManager::TransferItem(Player* pFrom, Player* pTo, int32_t nSlot, int32_t nCount)
{
    if (!pFrom || !pTo) return false;

    CExtCharDepot* pFromDepot = GetDepot(pFrom);
    CExtCharDepot* pToDepot   = GetDepot(pTo);

    if (!pFromDepot || !pToDepot) return false;

    // 获取源物品
    const MemChrBag* pSlotData = pFromDepot->GetSlotData(nSlot);
    if (!pSlotData || pSlotData->itemCount <= 0) return false;

    int32_t nTransferCount = (nCount > 0 && nCount < pSlotData->itemCount) ? nCount : pSlotData->itemCount;

    MemChrBag item = *pSlotData;
    item.itemCount = nTransferCount;
    item.bind = 1; // 转移后绑定

    // 加入目标仓库
    if (!pToDepot->AddItem(&item))
        return false;

    // 从源仓库移除
    pFromDepot->RemoveItem(nSlot);

    return true;
}
