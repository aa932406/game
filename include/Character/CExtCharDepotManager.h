// CExtCharDepotManager.h — 仓库管理器 (Depot Manager)
// 服务器级单例，管理所有玩家的仓库实例
#ifndef CEXTCHARDEPOTMANAGER_H
#define CEXTCHARDEPOTMANAGER_H

#include "Character/CExtCharDepot.h"
#include "Game/Player.h"
#include "Game/PlayerDBData.h"
#include <map>

class CExtCharDepotManager
{
public:
    CExtCharDepotManager();
    ~CExtCharDepotManager();

    // ---------- 单例访问 ----------
    static CExtCharDepotManager* GetInstance();

    // ---------- 仓库实例管理 ----------
    /// 获取玩家的仓库（不存在时自动创建）
    CExtCharDepot* GetDepot(Player* pPlayer);

    /// 玩家登录时调用
    void OnPlayerLogin(Player* pPlayer);

    /// 玩家登出时清理
    void OnPlayerLogout(Player* pPlayer);

    /// 从数据库加载仓库数据
    void OnLoadFromDB(Player* pPlayer, const PlayerDBData* dbData);

    /// 保存仓库数据到数据库
    void OnSaveToDB(Player* pPlayer, PlayerDBData* dbData);

    // ---------- 全局更新 ----------
    /// 更新所有玩家仓库
    void OnUpdate(int64_t curTick);

    /// 所有玩家每日切换
    void OnDaySwitch(int32_t nDiffDays);

    // ---------- 跨玩家操作 ----------
    /// 在两名玩家仓库间转移物品
    bool TransferItem(Player* pFrom, Player* pTo, int32_t nSlot, int32_t nCount);

    /// 获取拥有仓库的总玩家数
    int32_t GetDepotCount() const { return (int32_t)m_mDepots.size(); }

private:
    /// 内部创建仓库实例
    CExtCharDepot* CreateDepot(Player* pPlayer);

    /// 内部销毁仓库实例
    void DestroyDepot(Player* pPlayer);

private:
    Answer::RwLock    m_rwLock;              // 读写锁
    std::map<int32_t, CExtCharDepot*> m_mDepots;  // playerId -> CExtCharDepot*
};

// 便捷访问宏/内联函数: 替换 Player::GetCharDepot(m_pPlayer)
inline CExtCharDepot* GetPlayerDepot(Player* pPlayer)
{
    return CExtCharDepotManager::GetInstance()->GetDepot(pPlayer);
}

#endif // CEXTCHARDEPOTMANAGER_H
