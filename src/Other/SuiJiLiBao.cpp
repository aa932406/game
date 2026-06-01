#include "Other/SuiJiLiBao.h"
#include "Other/CfgData.h"

SuiJiLiBao::SuiJiLiBao()
{
    m_nId = 0;
}

SuiJiLiBao::~SuiJiLiBao()
{
}

bool SuiJiLiBao::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const std::vector<CfgItemGiftRandom>* pCfg = CfgData::getItemGiftRandom(v3, id);
    if (!pCfg)
        return false;
    m_gifts = *pCfg;
    return true;

void SuiJiLiBao::broadcast()
{
    if (!m_pPlayer)
        return;
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (!packet)
        return;
    packet->writeInt32(1);
    for (auto it = m_gifts.begin(); it != m_gifts.end(); ++it)
    {
        packet->writeInt32(it->item);
        packet->writeInt8(it->type);
        packet->writeInt32(it->count);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    GameService::worldBroadcast(pGame, packet);
}
