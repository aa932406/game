#include "Other/CLevelRefining.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CLevelRefining::CLevelRefining()
    : CExtSystemBase()
{
}

CLevelRefining::~CLevelRefining()
{
}

void CLevelRefining::OnCleanUp()
{
}

void CLevelRefining::OnUpdate(int64_t curTick)
{
}

void CLevelRefining::OnDaySwitch(int32_t nDiffDays)
{
}

void CLevelRefining::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CLevelRefining::OnSaveToDB(PlayerDBData* dbData)
{
}

void CLevelRefining::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(769);
    procList->push_back(768);
}

int32_t CLevelRefining::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 768)
        return OnRefining(inPacket);
    return 0;
}

int32_t CLevelRefining::OnRefining(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (GetLeftTime() <= 0)
        return 10002;
    int32_t nLevel = m_pPlayer->getLevel();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const LevelRefinCfg* pCfg = CfgData::GetLevelRefinCfg(cfg, nLevel);
    if (!pCfg)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_LEVEL_REFINING))
        return 10002;
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, inPacket->getProc(), 0);
    return 0;
}

void CLevelRefining::SendRefinInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD8);
    if (!packet)
        return;
    packet->writeInt32(GetLeftTime());
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

int32_t CLevelRefining::GetLeftTime()
{
    int32_t Now = Unit::getNow(m_pPlayer);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    return CfgData::getServerStartDayZeroTime(cfg, SERVER_TYPE::SVT_NORMAL) + 259200 - Now;
}

void CLevelRefining::GetIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    ShowIcon icon;
    GetIconStu(&icon);
    IconList->push_back(icon);
}

void CLevelRefining::GetIconStu(ShowIcon* __return_ptr retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    if (!m_pPlayer)
        return;
    retstr->nId = 127;
    retstr->nState = GetLeftTime() > 0 ? 2 : 4;
    retstr->nLeftTime = -1;
    if (GetLeftTime() > 0)
        retstr->nEffects = 1;
}

void CLevelRefining::SendIcon()
{
    if (!m_pPlayer)
        return;
    ShowIcon icon;
    GetIconStu(&icon);
    Player::SendIconState(m_pPlayer, &icon);
}
