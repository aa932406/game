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
struct MemChrBagVector;
struct ItemData;
struct ItemDataList;
struct RateItem;

typedef std::vector<RateItem> RateItemList;
typedef std::vector<std::string> StringVector;

class CItemHelper
{
public:
    int32_t NeedUseBagCount(MemChrBagVector *p_Items);
    bool CanEnterXinMoBag(int32_t nId, int8_t nClass);
    int32_t GetItemType(int32_t nId, int8_t nClass);
    void parseItemString(MemChrBag &retstr, std::string * strItem);
    ItemDataList parseItemDataListString(std::string * strItems, bool bCombi);
    ItemData parseItemDataString(std::string * strItem);
    void parseRateItemDataString(RateItem &retstr, std::string * strItem);
    RateItemList parseRateItemDataListString(std::string * strItems);
};

#endif // _CITEMHELPER_H_
