#include "Config/CfgMysteryShopTable.h"
#include "Answer/Logger.h"

CfgMysteryShopTable::CfgMysteryShopTable()
{
    m_mMysteryShop.clear();
}

CfgMysteryShopTable::~CfgMysteryShopTable()
{
    m_mMysteryShop.clear();
}

bool CfgMysteryShopTable::Add(const CfgMysteryShop& shop)
{
    if (shop.nId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgMysteryShopTable::Add invalid id: %d", shop.nId);
        return false;
    }
    
    m_mMysteryShop[shop.nId] = shop;
    return true;
}

const CfgMysteryShop* CfgMysteryShopTable::GetShopGood(int32_t nId)
{
    auto it = m_mMysteryShop.find(nId);
    if (it != m_mMysteryShop.end())
    {
        return &it->second;
    }
    return nullptr;
}

void CfgMysteryShopTable::CleanUp()
{
    m_mMysteryShop.clear();
}
