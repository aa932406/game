#include "Other/CGongMing.h"
#include "Network/NetPacket.h"
#include <list>

class GameService
{
public:
    Answer::NetPacket* popNetpacket(int8_t connId, Answer::PackType type, uint16_t proc);
    void worldBroadcast(int8_t connId, Answer::NetPacket* packet);
    int32_t replySuccess(int8_t connId, int16_t gateIndex, uint16_t proc, int32_t result);
    void sendPacketTo(int8_t connId, int16_t gateIndex, Answer::NetPacket* packet);
    static GameService* instance();
};

class CfgData
{
public:
    static CfgData* instance();
    static struct GongMingCfg* GetGongMingCfg(CfgData* self, int32_t level);
};

class CObjAttrs
{
public:
    enum Index_T : int32_t
    {
        ATTR_MAX_HP = 0,
        ATTR_ATTACK = 1,
        ATTR_DEFENSE = 2
    };
};

class CExtOperateLimit
{
public:
    void AddLimitCount(int32_t key, int32_t value);
};

struct AddAttribute
{
    int32_t m_nAddAttrType;
    int32_t m_nAddAttrValue;
};

struct GongMingCfg
{
    int32_t NeedGongMingZhi;
    int32_t GongGaoId;
    int32_t nChong;
    int32_t nStar;
    std::list<AddAttribute> lAttrList;
};

namespace Answer
{
    template<typename T>
    class Singleton
    {
    public:
        static T* instance();
    };
}

CGongMing::CGongMing()
{
}

CGongMing::~CGongMing()
{
}

void CGongMing::OnCleanUp()
{
}

void CGongMing::OnUpdate(int64_t curTick)
{
    (void)curTick;
}

void CGongMing::OnDaySwitch(int32_t nDiffDays)
{
    (void)nDiffDays;
}

void CGongMing::OnLoadFromDB(const PlayerDBData* dbData)
{
    (void)dbData;
}

void CGongMing::OnSaveToDB(PlayerDBData* dbData)
{
    (void)dbData;
}

void CGongMing::GetInterestsProtocol(ProcIdList* procList)
{
    if (procList)
        procList->push_back(262);
}

int32_t CGongMing::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    if (nProcId == 262)
        return OnGongMinLevelUp(inPacket);
    return 10002;
}

void CGongMing::AddCharAttr()
{
    if (!m_pPlayer)
        return;

    int32_t CurLevel = m_pPlayer->getRecord(1161);
    CfgData* v1 = CfgData::instance();
    GongMingCfg* CurCfg = CfgData::GetGongMingCfg(v1, CurLevel);
    if (!CurCfg)
        return;

    for (std::list<AddAttribute>::const_iterator it = CurCfg->lAttrList.begin(); it != CurCfg->lAttrList.end(); ++it)
    {
        int32_t value = it->m_nAddAttrValue;
        m_pPlayer->AddAttrValue((CObjAttrs::Index_T)it->m_nAddAttrType, value);
    }
}

void CGongMing::AddGongMingValues(int32_t Values)
{
    if (Values <= 0)
        return;

    CExtOperateLimit* OperateLimit = m_pPlayer->GetOperateLimit();
    OperateLimit->AddLimitCount(1162, Values);
    SendGongMinInfo();
}

int32_t CGongMing::OnGongMinLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;

    int32_t CurLevel = m_pPlayer->getRecord(1161);
    CfgData* v3 = CfgData::instance();
    GongMingCfg* CurCfg = CfgData::GetGongMingCfg(v3, CurLevel);
    CfgData* v4 = CfgData::instance();
    GongMingCfg* NextCfg = CfgData::GetGongMingCfg(v4, CurLevel + 1);
    int32_t HaveCurr = m_pPlayer->getRecord(1162);

    if (!CurCfg || !NextCfg)
        return 10002;
    if (CurCfg->NeedGongMingZhi > HaveCurr)
        return 10002;

    m_pPlayer->updateRecord(1162, HaveCurr - CurCfg->NeedGongMingZhi);
    m_pPlayer->updateRecord(1161, CurLevel + 1);

    if (NextCfg->GongGaoId > 0)
    {
        int8_t ConnId = m_pPlayer->getConnId();
        GameService* v6 = GameService::instance();
        Answer::NetPacket* packet = v6->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            packet->writeInt32(NextCfg->GongGaoId);
            std::string val = m_pPlayer->getName();
            packet->writeUTF8(val);
            CharId_t Cid = m_pPlayer->getCid();
            packet->writeInt64(Cid);
            packet->writeInt32(NextCfg->nChong);
            packet->writeInt32(NextCfg->nStar);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            int8_t v9 = m_pPlayer->getConnId();
            GameService* v10 = GameService::instance();
            v10->worldBroadcast(v9, packet);
        }
    }

    m_pPlayer->RecalcAttr();
    SendGongMinInfo();

    uint16_t Proc = inPacket->getProc();
    int16_t GateIndex = m_pPlayer->getGateIndex();
    int8_t v13 = m_pPlayer->getConnId();
    GameService* v14 = GameService::instance();
    return v14->replySuccess(v13, GateIndex, Proc, 0);
}

void CGongMing::SendGongMinInfo()
{
    if (!m_pPlayer)
        return;

    int32_t HaveCurr = m_pPlayer->getRecord(1162);
    int32_t CurLevel = m_pPlayer->getRecord(1161);
    CfgData* v1 = CfgData::instance();
    GongMingCfg* CurCfg = CfgData::GetGongMingCfg(v1, CurLevel);
    if (!CurCfg)
        return;

    int8_t ConnId = m_pPlayer->getConnId();
    GameService* v3 = GameService::instance();
    Answer::NetPacket* packet = v3->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2DCC);
    if (packet)
    {
        packet->writeInt32(CurCfg->nChong);
        packet->writeInt32(CurCfg->nStar);
        packet->writeInt32(HaveCurr);
        uint32_t WOffset = packet->getWOffset();
        packet->setSize(WOffset);
        int16_t GateIndex = m_pPlayer->getGateIndex();
        int8_t v6 = m_pPlayer->getConnId();
        GameService* v7 = GameService::instance();
        v7->sendPacketTo(v6, GateIndex, packet);
    }
}
