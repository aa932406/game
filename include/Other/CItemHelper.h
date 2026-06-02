#ifndef _CITEMHELPER_H_
#define _CITEMHELPER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
struct MemChrBag;
struct ItemData;
struct RateItem;

typedef std::vector<MemChrBag> MemChrBagVector;
typedef std::list<ItemData> ItemDataList;
typedef std::vector<RateItem> RateItemList;
typedef std::vector<std::string> StringVector;

#ifndef CURRENCY_ITEM_ID_DEFINED
#define CURRENCY_ITEM_ID_DEFINED
enum CURRENCY_ITEM_ID : int32_t
{
    CURRENCY_ITEM_MONEY     = 1,
    CURRENCY_ITEM_GOLD      = 2,
    CURRENCY_ITEM_BIND_GOLD = 3,
    CURRENCY_ITEM_HONOR     = 4,
    CURRENCY_ITEM_XUWU      = 5,
};
#endif

class CItemHelper
{
public:
    static int32_t NeedUseBagCount(MemChrBagVector *p_Items);
    static bool CanEnterXinMoBag(int32_t nId, int8_t nClass);
    static int32_t GetItemType(int32_t nId, int8_t nClass);
    static CURRENCY_TYPE TranseCurrencyType(CURRENCY_ITEM_ID id) {
        switch (id) {
        case CURRENCY_ITEM_MONEY: return CURRENCY_TYPE::CURRENCY_MONEY;
        case CURRENCY_ITEM_GOLD: return CURRENCY_TYPE::CURRENCY_GOLD;
        case CURRENCY_ITEM_BIND_GOLD: return CURRENCY_TYPE::CURRENCY_BIND_GOLD;
        case CURRENCY_ITEM_HONOR: return CURRENCY_TYPE::CURRENCY_HONOR;
        case CURRENCY_ITEM_XUWU: return CURRENCY_TYPE::CURRENCY_XUWU;
        default: return CURRENCY_TYPE::CURRENCY_MONEY;
        }
    }
    void parseItemString(MemChrBag &retstr, std::string * strItem);
    ItemDataList parseItemDataListString(std::string * strItems, bool bCombi);
    ItemData parseItemDataString(std::string * strItem);
    void parseRateItemDataString(RateItem &retstr, std::string * strItem);
    RateItemList parseRateItemDataListString(std::string * strItems);
};

#endif // _CITEMHELPER_H_
