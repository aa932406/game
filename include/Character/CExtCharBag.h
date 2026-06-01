// CExtCharBag.h
#ifndef CEXTCHARBAG_H
#define CEXTCHARBAG_H

#include "CExtSystemBase.h"
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstdint>

#ifndef MEM_CHR_BAG_DEFINED_HERE
#define MEM_CHR_BAG_DEFINED_HERE
struct MemChrBag
{
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};
#endif

#ifndef ITEM_DATA_DEFINED_HERE
#define ITEM_DATA_DEFINED_HERE
struct ItemData
{
    int32_t m_nId;
    int8_t  m_nClass;
    int32_t m_nCount;
};
#endif

struct LogItemChange
{
    int32_t cid;
    int8_t  flag;
    int32_t reason;
    int32_t id;
    int8_t  itemClass;
    int32_t count;
    int32_t time;
    int64_t srcId;
    int32_t allCount;
    char    name[32];
};

struct MemEquip;

typedef std::list<ItemData> ItemDataList;
typedef std::vector<MemChrBag> MemChrBagVector;
typedef std::vector<int> Int32Vector;

// ============== 变化原因枚举 ==============
#ifndef ITEM_CHANGE_REASON_DEFINED
#define ITEM_CHANGE_REASON_DEFINED
enum ITEM_CHANGE_REASON : int32_t
{
    ICR_NONE               = 0,
    ICR_PICK               = 127,
    ICR_SHOP_BUY           = 113,
    ICR_SELL               = 131,
    ICR_BUY_BACK           = 132,
    ICR_BAG_USE            = 133,
    ICR_ITEM_DISCARD       = 134,
    ICR_USE_MULTI_ITEM     = 135,
    ICR_DIE_DROP           = 140,
    ICR_MAIL               = 300,
    ICR_AUCTION_BUY        = 400,
    ICR_AUCTION_CANCEL     = 401,
    ICR_AUCTION_BACK       = 402,
    ICR_SELECT_ITEM        = 200,
    ICR_DUNGEON_REWARD     = 100,
    ICR_CROSS_DRAW_REWARD  = 500,
    ICR_DUNGEON_RANDOM     = 600,
    ICR_WORLD_BOSS_KILL_REWARD = 700,
};
#endif

// ============== 货币物品ID ==============
#ifndef CURRENCY_ITEM_ID_DEFINED
#define CURRENCY_ITEM_ID_DEFINED
enum CURRENCY_ITEM_ID : int32_t
{
    CURRENCY_ITEM_MONEY     = 1,
    CURRENCY_ITEM_GOLD      = 2,
    CURRENCY_ITEM_BIND_GOLD = 3,
    CURRENCY_ITEM_HONOR     = 4,
    CURRENCY_ITEM_XUWU      = 5,
    CURRENCY_ITEM_C6        = 6,
};
#endif

// ============== 货币类型 ==============
// 已由 CommonTypes.h 的 enum class CURRENCY_TYPE 替代

// ============== 货币变化原因 ==============
// 已由 CommonTypes.h/Map.h 的 enum class CURRENCY_CHANGE_REASON 替代

class CExtCharBag : public CExtSystemBase
{
public:
    struct BagDirty {
        int32_t nSlot;
        int8_t  bChange;
    };

    struct WaitCreate {
        int32_t nSlot;
        int32_t nReason;
    };

public:
    CExtCharBag();
    virtual ~CExtCharBag();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    void reset();
    void cleanSellItem();

    // 网络协议处理器
    int32_t onUseItem(Answer::NetPacket* inPacket);
    int32_t onPatchUseItem(Answer::NetPacket* inPacket);
    int32_t onSelectItem(Answer::NetPacket* inPacket);
    int32_t onDiscardItem(Answer::NetPacket* inPacket);
    int32_t onUseMutiItem(Answer::NetPacket* inPacket);
    int32_t onMoveItem(Answer::NetPacket* inPacket);
    int32_t onSplitItem(Answer::NetPacket* inPacket);
    int32_t onSort(Answer::NetPacket* inPacket);
    int32_t onSellItem(Answer::NetPacket* inPacket);
    int32_t onAddItem(Answer::NetPacket* inPacket);

    // 商业操作
    int32_t buyChrShopItem(void* pCharShop, int32_t id, int32_t count);
    int32_t buyBackChrShopItem(void* pCharShop, int32_t index);

    // 物品清理
    void CleanItemId(int32_t nId, int32_t nReason, int8_t nClass);

    // 限购系统
    int32_t getLimitCount(int32_t index);
    void addLimitCount(int32_t index, int32_t count);
    void SendLimitCount();

    // 查询接口
    const MemChrBag* GetSlotData(int32_t slot) const;
    int32_t GetTypeItem(int32_t nType);
    int32_t GetItemCount(int32_t nId, int8_t nClass) const;
    int32_t GetItemCount(const Int32Vector* vSlot, int8_t nClass, int32_t nId) const;
    bool HasItem(int32_t nId, int8_t nClass, int32_t nCount) const;
    bool HasItem(const ItemData* data) const;
    int32_t HaveItemCount(const ItemData* data, bool IsBind, bool IsTimeLiness) const;
    int32_t GetFirstFreeSlot() const;
    int32_t GetFreeSlotCount() const;
    bool IsSlotValid(int32_t slot) const;
    bool IsEmptySlot(const MemChrBag* slot) const;
    int32_t GetItemType(const MemChrBag* item) const;
    void GetItemSlot(Int32Vector* retstr, int8_t ItemClass, int32_t ItemId, int32_t ExcludeSlot) const;
    void GetItemSlot(Int32Vector* retstr, int8_t ItemClass, int32_t ItemId, int32_t* Count, bool IsBind) const;

    // 槽位操作
    void SetSlotData(int32_t slot, const MemChrBag* slotData, ITEM_CHANGE_REASON reason, int32_t count);
    void CleanSlot(int32_t slot, ITEM_CHANGE_REASON reason);
    int32_t GetBagSize() const;

    // 发送到客户端
    void SendBagItem();
    void SendBagSellItem();
    void sendGoldCashChange(int32_t type, int32_t addon, int32_t benefitType);

    // DB保存
    void SaveDBData(void* dbData);

    // 出售回购
    void setSellItem(const MemChrBag* sellItem);
    void deleteSellItem(int32_t index);

    // 添加/移除 (公开包装器)
    bool AddItem(const MemChrBagVector* vItem, ITEM_CHANGE_REASON nReason);
    bool AddItem(const MemChrBag* item, ITEM_CHANGE_REASON nReason);
    bool RemoveItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool RemoveItem(const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool RemoveItem(const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool RemoveItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool RemoveItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason, int32_t* BindCount, int32_t* UnBindCount);
    bool RemoveItem(const ItemData* data, ITEM_CHANGE_REASON nReason, int32_t* BindCount, int32_t* UnBindCount);
    bool RemoveItem(const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason, int32_t* BindCount, int32_t* UnBindCount);
    bool RemoveItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason, int32_t* BindCount, int32_t* UnBindCount);
    bool RemoveCombiItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason);

    // 批量增减 (先删后加, 事务性)
    bool AddAndRemoveItem(const MemChrBag* item, const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBag* item, const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBag* item, const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBag* item, const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBagVector* vItem, const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBagVector* vItem, const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBagVector* vItem, const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBagVector* vItem, const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool AddAndRemoveItem(const MemChrBagVector* vItem, const MemChrBagVector* vCost, ITEM_CHANGE_REASON nReason);

    // 使用检查
    bool canUseItem(const void* pCfgItem);

    // 备份/恢复 (事务支持)
    void backUpBagData();
    void recoverBagData();

    // 日志
    void addItemChangeLog(ITEM_CHANGE_REASON reason, int32_t itemId, int8_t itemClass, int64_t srcId, int32_t count, int8_t flag, int32_t allCount);
    void saveItemChangeLog();
    void clearItemChangeLog();

    // 脏标记同步
    void addDirty(int32_t nSlot, int32_t reason, bool bNeedCreate);
    void ForceSendDirty();
    void checkDirty();
    void clearDirty();
    bool sendDirty();

    // 自动使用
    bool autoUseItem(const MemChrBag* item, ITEM_CHANGE_REASON nReason);
    bool isAutoUseItem(const MemChrBag* item) const;

    // 广播
    void sendUseBroadcast(int32_t broadcastId, const std::string* pName, int64_t cid, int32_t itemId);

    // 掉落
    void GetDropItem(Player* pKiller, void* DropRate, int32_t nUsualRate, MemChrBagVector* ItemVector, int32_t Mid);

    // 其他公开方法
    bool AddItemsAndMingGe(const MemChrBagVector* vItem, ITEM_CHANGE_REASON nReason);

    // 排序比较器
    static bool GreaterItem(const MemChrBag& left, const MemChrBag& right);

private:
    bool addItem(const MemChrBag* item, ITEM_CHANGE_REASON nReason);
    bool addItem(const MemChrBagVector* vItem, ITEM_CHANGE_REASON nReason);
    bool removeItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool removeItem(const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool removeItem(const Int32Vector* vSlot, const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool removeItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason);
    bool removeItem(const ItemData* data, ITEM_CHANGE_REASON nReason, int32_t* BindCount, int32_t* UnBindCount);
    bool removeItem(const Int32Vector* vSlot, const ItemData* data, ITEM_CHANGE_REASON nReason, int32_t* BindCount, int32_t* UnBindCount);
    bool removeCombiItem(const ItemDataList* lst, ITEM_CHANGE_REASON nReason);
    bool compairItem(const MemChrBag* bagSlot, const ItemData* data) const;
    bool compairSlot(const MemChrBag* left, const MemChrBag* right) const;
    void setSlotData(int32_t slot, const MemChrBag* slotData, ITEM_CHANGE_REASON reason, int32_t count);

    // 数据成员
    static const int32_t BAG_SIZE = 56;
    static const int32_t SELL_SIZE = 15;
    MemChrBag m_bagData[BAG_SIZE];
    MemChrBag m_sellItemInfo[SELL_SIZE];
    MemChrBag m_nullobj;
    int64_t m_lastItemTick[8];
    MemChrBag m_bagBackUp[BAG_SIZE];
    std::list<LogItemChange> m_lstItemChangeLog;
    std::list<BagDirty> m_lstDirty;
    std::list<WaitCreate> m_lstWaitCreate;
    std::map<int32_t, int32_t> m_ItemLimit;
};

#endif