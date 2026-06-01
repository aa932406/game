// CExtCharDepot.h — 角色仓库系统 (Character Depot/Storage)
#ifndef CEXTCHARDEPOT_H
#define CEXTCHARDEPOT_H

#include "CExtSystemBase.h"
#include "CExtCharBag.h"
#include <list>
#include <string>
#include <cstdint>

class CExtCharDepot : public CExtSystemBase
{
public:
    static const int32_t BASE_SLOTS    = 56;
    static const int32_t MAX_SLOTS     = 560;
    static const int32_t CURRENCY_TYPE_COUNT = 12;

public:
    CExtCharDepot();
    virtual ~CExtCharDepot();

    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;

    // ---------- 核心查询 ----------
    int32_t GetDepotSize() const;
    const MemChrBag* GetSlotData(int32_t slot) const;
    bool IsSlotValid(int32_t slot) const;
    int32_t GetItemCount(int32_t nId, int8_t nClass) const;
    int32_t GetPageBySlot(int32_t slot) const;

    // ---------- 槽位操作 ----------
    bool setSlotData(int32_t slot, const MemChrBag* slotData);
    bool RemoveItem(int32_t slot);
    bool AddItem(const MemChrBag* item);
    void CleanBag();

    // ---------- 货币 ----------
    int64_t GetCurrency(int32_t nType) const;
    void SendDepotCurrency();
    void CheckCurrency();

    // ---------- 脏标记 ----------
    void AddDirty(int32_t nSlot);
    void CheckDirty();
    bool SendDirty();
    void ClearDirty();

    // ---------- 网络发送 ----------
    void SendDepotItem();
    void SendDepotInfo();

    // ---------- 访问控制 ----------
    bool CanOperateDepot();
    bool IsEnterSecondPasswordOperate();

    // ---------- 网络协议处理器 ----------
    int32_t OnGetDepotItem(Answer::NetPacket* inPacket);
    int32_t OnSaveItemToDepot(Answer::NetPacket* inPacket);
    int32_t OnGetDepotCurrency(Answer::NetPacket* inPacket);
    int32_t OnSaveDepotCurrency(Answer::NetPacket* inPacket);
    int32_t OnNewSortDepot(Answer::NetPacket* inPacket);
    int32_t OnSortDepot(Answer::NetPacket* inPacket);
    int32_t OnOpenDepotSlot(Answer::NetPacket* inPacket);
    int32_t OpenSlotCostGold(int32_t slot);

    // ---------- 密码处理器 ----------
    int32_t OnSetPassword(Answer::NetPacket* inPacket);
    int32_t OnEnterPassword(Answer::NetPacket* inPacket);
    int32_t OnModifyPassword(Answer::NetPacket* inPacket);
    int32_t OnCancelPassword(Answer::NetPacket* inPacket);
    int32_t OnSetSecondPassword(Answer::NetPacket* inPacket);
    int32_t OnEnterSecondPassword(Answer::NetPacket* inPacket);
    int32_t OnModifySecondPassword(Answer::NetPacket* inPacket);
    int32_t OnCancelSecondPassword(Answer::NetPacket* inPacket);

    // ---------- 比较器 ----------
    bool compairSlot(const MemChrBag* left, const MemChrBag* right) const;

private:
    int32_t       m_OpenCount;            // 已额外打开的槽位数
    bool          m_NeedSendCurrency;     // 需要发送货币更新
    bool          m_NeedSendDirty;        // 需要发送脏标记
    std::string   m_Password;             // 一级密码
    std::string   m_SendPassword;         // 二级密码
    MemChrBag     m_nullobj;              // 空对象标记
    int64_t       m_aCurrency[12];        // 仓库货币 [12 种]
    MemChrBag     m_DepotData[560];       // 仓库物品槽 (56 ~ 560 格)
    std::list<int32_t> m_lstDirty;        // 脏槽位列表
};

// 排序比较器 (free function, 用于 std::sort)
bool sortItem(const MemChrBag& left, const MemChrBag& right);

#endif // CEXTCHARDEPOT_H
