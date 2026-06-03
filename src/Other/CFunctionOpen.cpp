#include "common.h"
#include "Other/CFunctionOpen.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CFunctionOpen::CFunctionOpen()
{
}

CFunctionOpen::~CFunctionOpen()
{
}

void CFunctionOpen::OnCleanUp()
{
    m_OpenedList.clear();
    m_SendMailList.clear();
}

void CFunctionOpen::OnUpdate(int64_t curTick)
{
}

void CFunctionOpen::OnDaySwitch(int32_t nDiffDays)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) != 9)
    {
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        int32_t v4 = CfgData::getServerDiffDay(cfg, SERVER_TYPE::SVT_NORMAL) + 1;
        CheckFunctionOpenMailByKaiFuDay(v4);
    }
}

void CFunctionOpen::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CFunctionOpen::OnSaveToDB(PlayerDBData* dbData)
{
}

void CFunctionOpen::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(470);
}

int32_t CFunctionOpen::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 470)
        return OnGetSysOpenGift(inPacket);
    return 2;
}

int32_t CFunctionOpen::OnGetSysOpenGift(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int32_t Id = inPacket->readInt32();
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const SystemOpenGift* pCfg = CfgData::GetSystemOpenGift(cfg, Id);
    if (!pCfg)
        return 2;
    if (!IsOpened(pCfg->TypeId))
        return 2;
    if (pCfg->Star > Player::getRecord(m_pPlayer, 1148))
        return 2;

    int32_t Record = Player::getRecord(m_pPlayer, 1151);
    int32_t NewRecord = Record | (1 << (Id - 1));
    if (NewRecord == Record)
        return 10002;

    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->Gifts, ITEM_CHANGE_REASON::ICR_SYS_OPEN_GIFT))
        return 10002;

    Player::updateRecord(m_pPlayer, 1151, NewRecord);
    SendSysOpenIcon();

    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, connId, gateIndex, Proc, 0);
    return 0;
}

void CFunctionOpen::GetSysOpenIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    ShowIcon icon;
    GetSysOpenIcon(&icon);
    IconList->push_back(icon);
}

void CFunctionOpen::SendSysOpenIcon()
{
    if (!m_pPlayer)
        return;
    ShowIcon icon;
    GetSysOpenIcon(&icon);
    Player::SendIconState(m_pPlayer, &icon);
}

void CFunctionOpen::GetSysOpenIcon(ShowIcon* retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    if (!m_pPlayer)
        return;
    retstr->nId = 115;
    retstr->nState = IsAllRewardGet() ? 4 : 2;
    retstr->nLeftTime = -1;
    retstr->nIconRight = CanGetRewardCount();
    if (retstr->nIconRight > 0)
        retstr->nEffects = 1;
}

bool CFunctionOpen::IsAllRewardGet()
{
    int32_t OldRecord = Player::getRecord(m_pPlayer, 1151);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const SystemOpenGiftMap* SystemOpenMap = CfgData::GetSystemOpenMap(cfg);
    SystemOpenGiftMap SystemOpenGifts(*SystemOpenMap);
    for (auto& pair : SystemOpenGifts)
    {
        int32_t NewRecord = OldRecord | (1 << (static_cast<uint8_t>(pair.first) - 1));
        if (NewRecord != OldRecord)
            return false;
    }
    return true;
}

int32_t CFunctionOpen::CanGetRewardCount()
{
    int32_t Count = 0;
    int32_t OldRecord = Player::getRecord(m_pPlayer, 1151);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const SystemOpenGiftMap* SystemOpenMap = CfgData::GetSystemOpenMap(cfg);
    SystemOpenGiftMap SystemOpenGifts(*SystemOpenMap);
    for (auto& pair : SystemOpenGifts)
    {
        int32_t NewRecord = OldRecord | (1 << (static_cast<uint8_t>(pair.first) - 1));
        if (NewRecord != OldRecord && IsOpened(pair.second.TypeId))
            ++Count;
    }
    return Count;
}

void CFunctionOpen::InitFunctionOpen(int32_t TaskId, int32_t Level)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const FunctionOpenCfgMap* OpenFunctionTable = CfgData::GetOpenFunctionTable(cfg);
    FunctionOpenCfgMap CfgTable(*OpenFunctionTable);
    for (auto& pair : CfgTable)
    {
        const FunctionOpenCfg& cfgEntry = pair.second;
        if (TaskId <= 0)
        {
            if (cfgEntry.TaskId <= 0 && cfgEntry.Level <= Level)
                m_OpenedList.push_back(pair.first);
        }
        else if (cfgEntry.TaskId > 0)
        {
            CfgTask* pTask = CfgData::getTask(cfg, cfgEntry.TaskId);
            if (pTask)
            {
                CfgTask* pCfgLastTrunk = CfgData::getTask(cfg, TaskId);
                if (pCfgLastTrunk && pCfgLastTrunk->main_order > pTask->main_order)
                    m_OpenedList.push_back(pair.first);
            }
        }
    }
}

void CFunctionOpen::CheckFunctionOpne(int32_t TaskId, int32_t Level)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const FunctionOpenCfgMap* OpenFunctionTable = CfgData::GetOpenFunctionTable(cfg);
    FunctionOpenCfgMap CfgTable(*OpenFunctionTable);
    std::list<int> NewOpenList;

    for (auto& pair : CfgTable)
    {
        const FunctionOpenCfg& cfgEntry = pair.second;
        if (TaskId <= 0)
        {
            if (cfgEntry.TaskId <= 0 && cfgEntry.Level <= Level)
            {
                int first = pair.first;
                if (!IsOpened(first))
                {
                    m_OpenedList.push_back(first);
                    NewOpenList.push_back(first);
                }
            }
        }
        else if (cfgEntry.TaskId > 0)
        {
            CfgTask* pTask = CfgData::getTask(cfg, cfgEntry.TaskId);
            if (pTask)
            {
                CfgTask* pCfgLastTrunk = CfgData::getTask(cfg, TaskId);
                if (pCfgLastTrunk && pCfgLastTrunk->main_order >= pTask->main_order)
                {
                    int first = pair.first;
                    if (!IsOpened(first))
                    {
                        m_OpenedList.push_back(first);
                        NewOpenList.push_back(first);
                    }
                }
            }
        }
    }

    for (int id : NewOpenList)
        FunctionInit(id);
}

void CFunctionOpen::FunctionInit(int32_t FunctionId)
{
    if (!m_pPlayer)
        return;

    switch (FunctionId)
    {
    case 25:
        Player::GetCharTitle(m_pPlayer)->InitTitle();
        break;
    case 26:
        Player::GetCharTaskCycle(m_pPlayer)->OpenCycleTask();
        break;
    case 32:
        Player::GetCharPet(m_pPlayer)->OnInitPet();
        break;
    case 53:
        Answer::Singleton<CWorldBoss>::instance()->SendWorldBossIcon(m_pPlayer);
        break;
    case 57:
        Player::GetPlayerQiFu(m_pPlayer)->SendQIFuIcon();
        break;
    case 59:
        Player::GetPlayerDailyActivity(m_pPlayer)->SendHuoDongDaTingIcon();
        break;
    case 60:
    case 108:
        Player::GetPlayerDailyActivity(m_pPlayer)->SendJiangLiDaTingIcon();
        break;
    case 62:
        Player::GetPlayerYunYingHd(m_pPlayer)->SendShouChongIcon();
        break;
    case 63:
        Player::GetPlayerYunYingHd(m_pPlayer)->SendTeHuiIcon();
        break;
    case 64:
        Player::GetPlayerYunYingHd(m_pPlayer)->SendEveryDayChongZhiIcon(3);
        Player::GetPlayerYunYingHd(m_pPlayer)->SendEveryDayChongZhiIcon(2);
        break;
    case 65:
        Player::GetPlayerGetTouZi(m_pPlayer)->SendTouZiIcon();
        break;
    case 66:
        Answer::Singleton<CKaiFuHuoDong>::instance()->SendKaiFuHuoDongIcon(m_pPlayer);
        Answer::Singleton<CKaiFuHuoDong>::instance()->SendKaiFuPetIcon(m_pPlayer);
        break;
    case 68:
        Player::GetCharHallOfFame(m_pPlayer)->SendIconState(0);
        break;
    case 112:
        Player::GetPlayerDailyActivity(m_pPlayer)->SendSevenDayLoginIcon();
        break;
    case 140:
        Player::GetCharWing(m_pPlayer)->OnFunctionOpen();
        break;
    case 167:
        Player::SendDungeonHallIcon(m_pPlayer);
        break;
    case 168:
        Player::GetPlayerDailyActivity(m_pPlayer)->SendSearchBackIcon();
        break;
    case 173:
        Player::SendQuestionAnswerIcon(m_pPlayer);
        break;
    case 175:
        Player::GetTianLing(m_pPlayer)->InitTianLing();
        break;
    case 177:
        Player::GetPlayerChouJiang(m_pPlayer)->SendHuoDongIcon();
        break;
    case 178:
        Player::GetCMoneyRewardTask(m_pPlayer)->OnFunctionOpen();
        break;
    case 179:
        Player::GetCMoneyRewardTask(m_pPlayer)->OnPdbfFunctionOpen();
        break;
    case 180:
        Player::GetCMoneyRewardTask(m_pPlayer)->OnBackEquipFunctionOpen();
        break;
    case 181:
        Player::GetCSpecialEquip(m_pPlayer)->SendSpecialEquipIcon();
        break;
    case 186:
        Player::GetCLevelRefining(m_pPlayer)->SendIcon();
        break;
    case 188:
        Player::RecalcAttr(m_pPlayer);
        break;
    case 190:
        Player::GetCBossKilledReward(m_pPlayer)->SendBossKilledIcon();
        break;
    case 192:
        Player::GetCTestServerReward(m_pPlayer)->SendTestServerIcon();
        break;
    case 193:
        Player::GetCMoneyRewardTask(m_pPlayer)->OnXiangYaoFunctionOpen();
        break;
    case 194:
        Player::GetCMoneyRewardTask(m_pPlayer)->OnShenWeiFunctionOpen();
        break;
    case 195:
        Player::sendJoinQQIcon(m_pPlayer);
        break;
    case 197:
        Player::GetPlayerHuoYueDu(m_pPlayer)->SendtHuoYueDuIcon();
        break;
    case 201:
        Player::GetCSuperTeHui(m_pPlayer)->SendSuperTeHuiIcon();
        break;
    case 210:
        Player::GetCMonthlyChouJiang(m_pPlayer)->SendIcon();
        break;
    case 211:
        Player::GetWan360(m_pPlayer)->SendWeiXinIcon();
        break;
    case 212:
        Player::GetWan360(m_pPlayer)->SendGameAppIcon();
        break;
    case 235:
        Player::GetTianLing(m_pPlayer)->InitZhanHun();
        break;
    case 237:
        Player::GetPlayerYunYingHd(m_pPlayer)->OnZeroBuyPetOpen();
        break;
    case 240:
        Player::GetTianLing(m_pPlayer)->InitSunAndMoon();
        break;
    default:
        break;
    }
    SendSysOpenIcon();
}

bool CFunctionOpen::IsOpened(int32_t FunctionId)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (!CfgData::GetOpenFunctionCfg(cfg, FunctionId))
        return true;
    for (int id : m_OpenedList)
    {
        if (id == FunctionId)
            return true;
    }
    return false;
}

void CFunctionOpen::InitFunctionOpenMailByLevel(int32_t nLevel)
{
    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9)
        return;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const FunctionOpenMailMap* OpenFunctionMailTable = CfgData::GetOpenFunctionMailTable(cfg);
    FunctionOpenMailMap CfgTable(*OpenFunctionMailTable);
    for (auto& pair : CfgTable)
    {
        if (pair.second.nLevel > 0 && pair.second.nLevel <= nLevel)
            AddSendMailList(pair.first);
    }
}

void CFunctionOpen::InitFunctionOpenMailByKaiFuDay(int32_t KaiFuDay)
{
    if (!m_pPlayer)
        return;

    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9)
        return;

    int32_t nId = Player::getRecord(m_pPlayer, 1916);
    if (nId <= 0)
        return;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const FunctionOpenMailMap* OpenFunctionMailTable = CfgData::GetOpenFunctionMailTable(cfg);
    FunctionOpenMailMap CfgTable(*OpenFunctionMailTable);
    for (auto& pair : CfgTable)
    {
        if (pair.second.nLevel <= 0 && pair.first <= nId)
            AddSendMailList(pair.first);
    }
    Player::updateRecord(m_pPlayer, 1916, 0);
}

void CFunctionOpen::CheckFunctionOpenMailByLevel(int32_t nLevel)
{
    if (!m_pPlayer)
        return;

    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9)
        return;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const FunctionOpenMailMap* OpenFunctionMailTable = CfgData::GetOpenFunctionMailTable(cfg);
    FunctionOpenMailMap CfgTable(*OpenFunctionMailTable);
    for (auto& pair : CfgTable)
    {
        if (pair.second.nLevel > 0)
        {
            if (!IsSendedMail(pair.first) && pair.second.nLevel <= nLevel)
            {
                AddSendMailList(pair.first);
                int32_t nMailID = pair.second.nMailID;
                CharId_t Cid = Player::getCid(m_pPlayer);
                DBService* db = Answer::Singleton<DBService>::instance();
                std::string Param;
                DBService::OnSendSysMail(db, 0, Cid, nMailID, &Param, 0);
            }
        }
    }
}

void CFunctionOpen::CheckFunctionOpenMailByKaiFuDay(int32_t nKaiFuDay)
{
    if (!m_pPlayer)
        return;

    GameService* gs = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(gs) == 9)
        return;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const FunctionOpenMailMap* OpenFunctionMailTable = CfgData::GetOpenFunctionMailTable(cfg);
    FunctionOpenMailMap CfgTable(*OpenFunctionMailTable);
    for (auto& pair : CfgTable)
    {
        if (pair.second.nLevel <= 0)
        {
            if (!IsSendedMail(pair.first) && pair.second.nDay <= nKaiFuDay)
            {
                if (m_pPlayer->getLevel() >= pair.second.nDayLevel)
                {
                    AddSendMailList(pair.first);
                    int32_t nMailID = pair.second.nMailID;
                    CharId_t Cid = Player::getCid(m_pPlayer);
                    DBService* db = Answer::Singleton<DBService>::instance();
                    std::string Param;
                    DBService::OnSendSysMail(db, 0, Cid, nMailID, &Param, 0);
                }
            }
        }
    }
}

void CFunctionOpen::AddSendMailList(int32_t nId)
{
    if (nId > 0 && nId <= 199)
        Player::updateRecord(m_pPlayer, nId + 31100, 1);
}

bool CFunctionOpen::IsSendedMail(int32_t Id)
{
    return Player::getRecord(m_pPlayer, Id + 31100) > 0;
}
