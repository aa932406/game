#include "Other/CItemHelper.h"
#include "Common/CommonTypes.h"
#include "Config/CfgData.h"
#include "Character/CExtCharBag.h"

#include <cstring>
#include <algorithm>
#include <sstream>

int32_t CItemHelper::NeedUseBagCount(MemChrBagVector *p_Items)
{
    int32_t Count = 0;
    for (size_t i = 0; i < p_Items->size(); ++i)
    {
        Count += (*p_Items)[i].itemCount;
    }
    return Count;
}

bool CItemHelper::CanEnterXinMoBag(int32_t nId, int8_t nClass)
{
    return false;
}

int32_t CItemHelper::GetItemType(int32_t nId, int8_t nClass)
{
    return 0;
}

static StringVector splitString(const std::string &str, char delim)
{
    StringVector tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void CItemHelper::parseItemString(MemChrBag &retstr, std::string * strItem)
{
    StringVector items = splitString(*strItem, ',');
    if (items.size() >= 3)
    {
        retstr.itemId = std::stoi(items[0]);
        retstr.itemClass = std::stoi(items[1]);
        retstr.itemCount = std::stoi(items[2]);
        if (items.size() >= 4)
        {
            retstr.bind = std::stoi(items[3]);
        }
        else
        {
            retstr.bind = 0;
        }
    }
}

ItemDataList CItemHelper::parseItemDataListString(std::string * strItems, bool bCombi)
{
    ItemDataList itemList;
    StringVector items = splitString(*strItems, ';');
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (!items[i].empty())
        {
            ItemData item = parseItemDataString(&items[i]);
            if (bCombi)
            {
                bool bFound = false;
                for (auto it = itemList.begin(); it != itemList.end(); ++it)
                {
                    if (it->m_nId == item.m_nId && it->m_nClass == item.m_nClass)
                    {
                        it->m_nCount += item.m_nCount;
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    itemList.push_back(item);
                }
            }
            else
            {
                itemList.push_back(item);
            }
        }
    }
    return itemList;
}

ItemData CItemHelper::parseItemDataString(std::string * strItem)
{
    ItemData item;
    StringVector parts = splitString(*strItem, ',');
    if (parts.size() >= 1)
    {
        item.m_nId = std::stoi(parts[0]);
    }
    if (parts.size() >= 2)
    {
        item.m_nClass = std::stoi(parts[1]);
    }
    if (parts.size() >= 3)
    {
        item.m_nCount = std::stoi(parts[2]);
    }
    return item;
}

void CItemHelper::parseRateItemDataString(RateItem &retstr, std::string * strItem)
{
    StringVector parts = splitString(*strItem, ',');
}

RateItemList CItemHelper::parseRateItemDataListString(std::string * strItems)
{
    RateItemList itemList;
    StringVector items = splitString(*strItems, ';');
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (!items[i].empty())
        {
            RateItem item;
            parseRateItemDataString(item, &items[i]);
            itemList.push_back(item);
        }
    }
    return itemList;
}
