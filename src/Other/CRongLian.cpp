#include "Other/CRongLian.h"
#include "Answer.h"
#include "GameService.h"

CRongLian::CRongLian(const CfgActivity* cfgActivity)
    : CActivity(cfgActivity)
{
}

CRongLian::~CRongLian()
{
}

void CRongLian::broadcastStart()
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (packet)
    {
        packet->writeInt32(522);
        packet->setSize(packet->getWOffset());
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, 0, packet);
    }
}

void CRongLian::sendPlayerScore(Player* player)
{
    if (player)
    {
        int8_t connId = player->getConnId();
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E26u);
        if (packet)
        {
            packet->writeInt32(m_cfgActivity->id);
            packet->writeInt32(getLeftTime());
            packet->setSize(packet->getWOffset());
            int16_t gateIndex = player->getGateIndex();
            int8_t connId2 = player->getConnId();
            GameService* gs2 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
        }
    }
}
