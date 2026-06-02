// CExtCharMysteryShop.cpp
#include "CExtCharMysteryShop.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharMysteryShop::CExtCharMysteryShop()
{
    OnCleanUp();
}

CExtCharMysteryShop::~CExtCharMysteryShop()
{
}

void CExtCharMysteryShop::OnCleanUp()
{
    m_lstShops.clear();
}

void CExtCharMysteryShop::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_lstShops = dbData->m_MysteryShopDBData.lstShop;
    }
}

void CExtCharMysteryShop::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_MysteryShopDBData.lstShop = m_lstShops;
    }
}

void CExtCharMysteryShop::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 881;
    ProcId_t v4 = 882;
    procList->push_back(v3);
    procList->push_back(v4);
}

int32_t CExtCharMysteryShop::onAskShopInfo(Answer::NetPacket* inPacket)
{
    int8_t nType = 0;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    return 0;
}

int32_t CExtCharMysteryShop::onBuyShopItem(Answer::NetPacket* inPacket)
{
    int8_t nType = 0;
    int8_t nIndex = 0;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    return 0;
}

int32_t CExtCharMysteryShop::onRefreshShopItems(Answer::NetPacket* inPacket)
{
    int8_t nType = 0;
    
    if (!m_pPlayer || !inPacket) return 10002;
    
    return 0;
}

void CExtCharMysteryShop::sendShopInfo(const MysteryShop* shop)
{
    if (!m_pPlayer) return;
}

void CExtCharMysteryShop::checkRefresh(MysteryShop* shop)
{
    if (!m_pPlayer) return;
}

void CExtCharMysteryShop::refreshShop(MysteryShop* shop, bool bAuto)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharMysteryShop::getNextRefreshTime(int32_t nLastTime)
{
    return 7200 * (nLastTime / 7200 + 1);
}

int32_t CExtCharMysteryShop::refreshShopItem(int8_t nType, int32_t nLevel, void* excepts)
{
    return 0;
}

void CExtCharMysteryShop::OnUpdate(int64_t curTick)
{
}

void CExtCharMysteryShop::OnDaySwitch(int32_t nDiffDays)
{
}