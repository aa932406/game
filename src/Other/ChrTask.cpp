#include "Other/ChrTask.h"

#include <cstring>
#include <algorithm>

ChrTask::ChrTask()
{
    m_player = nullptr;
    m_trunkTaskId = 0;
}

ChrTask::~ChrTask()
{
}

void ChrTask::init(Player *player, MemChrTaskVector *tasks)
{
    if (!player)
        return;

    m_player = player;

    for (auto it = tasks->begin(); it != tasks->end(); ++it)
    {
        int32_t id = it->id;
        CfgData *pCfg = CfgData::instance();
        CfgTask *cfgTask = CfgData::getTask(pCfg, id);
        if (!cfgTask || cfgTask->type == 38 || cfgTask->type == 41)
            continue;

        if (cfgTask->type == 1)
        {
            int32_t v5 = m_trunkTaskId;
            CfgData *v6 = CfgData::instance();
            CfgTask *pCfgLastTrunk = CfgData::getTask(v6, v5);
            if (pCfgLastTrunk)
            {
                if (pCfgLastTrunk->main_order > cfgTask->main_order)
                    continue;
                m_tasks.erase(m_trunkTaskId);
                m_trunkTaskId = it->id;
            }
            else
            {
                m_trunkTaskId = cfgTask->id;
            }
        }

        MemChrTask &task = m_tasks[it->id];
        task = *it;

        if ((task.state == 3 || task.state == 4) && CheckTaskCondition(cfgTask->condition))
        {
            TaskMonster taskMonster;
            memset(&taskMonster, 0, sizeof(taskMonster));
            taskMonster.tid = it->id;
            taskMonster.condition = cfgTask->condition;
            taskMonster.request.param1 = cfgTask->request.param1;
            taskMonster.request.param2 = it->monster;
            m_taskMonsters.push_back(taskMonster);
        }
    }

    CFunctionOpen *PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_player);
    CFunctionOpen::InitFunctionOpen(PlayerFunctionOpen, m_trunkTaskId, 0);
}

bool ChrTask::CheckTaskCondition(int32_t taskCondition)
{
    return (uint32_t)taskCondition <= 0x2B && ((1LL << taskCondition) & 0xDFDDB7F2FB8LL) != 0;
}

void ChrTask::reset()
{
    m_player = nullptr;
    m_tasks.clear();
    m_trunkTaskId = 0;
    m_taskMonsters.clear();
}

int32_t ChrTask::receive(int32_t tid)
{
    if (!m_player)
        return 10002;

    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
    if (!cfgTask)
        return 10002;

    if (!receivePreCheck(tid, cfgTask))
        return 10002;

    CExtCharBag *Bag = Player::GetBag(m_player);
    if (!CExtCharBag::AddItem(Bag, &cfgTask->items_receive, ITEM_CHANGE_REASON::ICR_NONE))
        return 10002;

    if (cfgTask->type == 1)
    {
        m_tasks.erase(m_trunkTaskId);
    }
    else if (cfgTask->type == 37)
    {
        CleanTaskType(cfgTask->type);
    }
    else if (cfgTask->pretask)
    {
        int32_t type = cfgTask->type;
        if (type == GetTaskType(cfgTask->pretask))
            m_tasks.erase(cfgTask->pretask);
    }

    MemChrTask &task = m_tasks[tid];
    task.id = tid;
    task.state = 3;
    sendTaskReceived(tid);

    TaskMonster taskMonster;
    memset(&taskMonster, 0, sizeof(taskMonster));
    taskMonster.tid = tid;
    taskMonster.condition = cfgTask->condition;
    taskMonster.request.param1 = cfgTask->request.param1;
    m_taskMonsters.push_back(taskMonster);
    sendTaskKilledMonster();
    Player::updateLastTaskId(m_player, tid);

    if (cfgTask->type == 1)
    {
        m_trunkTaskId = tid;
    }

    updateTaskCount(18, 0);
    updateTaskCount(19, 0);
    updateTaskCount(20, 0);
    updateTaskCount(22, 0);
    updateTaskCount(28, 0);
    updateTaskCount(37, 0);
    updateTaskCount(38, 0);
    updateTaskCount(39, 0);

    if (cfgTask->BuffId > 0)
    {
        CfgData *pCfg2 = CfgData::instance();
        CfgBuff *cfgBuff = CfgData::getBuff(pCfg2, cfgTask->BuffId);
        if (cfgBuff)
        {
            UnitHandle launcher;
            launcher.id = m_player->getId();
            launcher.type = 1;
            Buff *buff = new Buff(m_player, cfgBuff, &launcher, 1, 1);
            if (buff)
                Unit::addBuff(m_player, buff);
        }
    }

    return 0;
}

bool ChrTask::ForceSubmit(int32_t tid)
{
    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
    if (!cfgTask)
        return false;

    auto it = m_tasks.find(tid);
    if (it == m_tasks.end())
        return false;
    if (it->second.state != 3 && it->second.state != 4)
        return false;

    MemChrBagVector add(cfgTask->award_item);

    for (auto &jobBag : cfgTask->award_optional)
    {
        Job_t Job = Player::getJob(m_player);
        if (Job == jobBag.job)
        {
            MemChrBag chrbag;
            memset(&chrbag, 0, sizeof(chrbag));
            chrbag.itemId = jobBag.id;
            chrbag.itemClass = jobBag.type;
            chrbag.itemCount = jobBag.count;
            chrbag.bind = jobBag.bind;
            add.push_back(chrbag);
        }
    }

    CExtCharBag *Bag = Player::GetBag(m_player);
    if (!CExtCharBag::AddItem(Bag, &add, ITEM_CHANGE_REASON::ICR_TASK_REWARD))
        return false;

    if (cfgTask->award_money > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_MONEY, cfgTask->award_money, CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->award_exp > 0)
        Player::addExp(m_player, cfgTask->award_exp, 0, 1);
    if (cfgTask->rongyu > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_HONOR, cfgTask->rongyu, CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);

    it->second.id = tid;
    it->second.state = 5;
    it->second.monster = 0;

    double bnfRatio = Player::benefitRatio(m_player);
    sendTaskSubmitted(tid, (int)((double)cfgTask->award_exp * bnfRatio));

    for (auto mit = m_taskMonsters.begin(); mit != m_taskMonsters.end(); ++mit)
    {
        if (mit->tid == tid)
        {
            m_taskMonsters.erase(mit);
            sendTaskKilledMonster();
            break;
        }
    }

    Player::updateLastTaskId(m_player, tid);
    CFunctionOpen *PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_player);
    CFunctionOpen::CheckFunctionOpne(PlayerFunctionOpen, tid, 0);

    if (cfgTask->type == 1 && cfgTask->posttask > 0)
    {
        int32_t postTaskId = cfgTask->posttask;
        CfgData *pCfg2 = CfgData::instance();
        CfgTask *cfgPostTask = CfgData::getTask(pCfg2, postTaskId);
        if (!cfgPostTask)
            return false;

        CExtCharBag *Bag2 = Player::GetBag(m_player);
        CExtCharBag::AddItem(Bag2, &cfgPostTask->items_receive, ITEM_CHANGE_REASON::ICR_NONE);
        m_tasks.erase(tid);

        MemChrTask &postTask = m_tasks[postTaskId];
        postTask.id = postTaskId;
        postTask.state = 3;
        sendTaskReceived(postTaskId);

        TaskMonster taskMonster;
        memset(&taskMonster, 0, sizeof(taskMonster));
        taskMonster.tid = postTaskId;
        taskMonster.condition = cfgPostTask->condition;
        taskMonster.request.param1 = cfgPostTask->request.param1;
        m_taskMonsters.push_back(taskMonster);
        sendTaskKilledMonster();
        Player::updateLastTaskId(m_player, postTaskId);
        m_trunkTaskId = postTaskId;

        updateTaskCount(18, 0);
        updateTaskCount(19, 0);
        updateTaskCount(20, 0);
        updateTaskCount(22, 0);
        updateTaskCount(28, 0);
        updateTaskCount(37, 0);
        updateTaskCount(38, 0);
        updateTaskCount(39, 0);
    }

    updateTaskTypeCount(cfgTask->type);
    return true;
}

int32_t ChrTask::submit(int32_t tid, int32_t nTimes)
{
    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
    if (!cfgTask)
        return 10002;

    if (!submitPreCheck(tid, cfgTask))
        return 10002;

    ItemDataList itemSubmit;
    if (cfgTask->condition == 2)
    {
        ItemData item;
        memset(&item, 0, sizeof(item));
        item.m_nId = cfgTask->request.param1;
        item.m_nClass = cfgTask->request.param2;
        item.m_nCount = cfgTask->request.param3;
        itemSubmit.push_back(item);
    }

    MemChrBagVector add(cfgTask->award_item);
    for (auto &bag : add)
        bag.itemCount *= nTimes;

    for (auto &jobBag : cfgTask->award_optional)
    {
        Job_t Job = Player::getJob(m_player);
        if (Job == jobBag.job)
        {
            MemChrBag chrbag;
            memset(&chrbag, 0, sizeof(chrbag));
            chrbag.itemId = jobBag.id;
            chrbag.itemClass = jobBag.type;
            chrbag.itemCount = jobBag.count;
            chrbag.bind = jobBag.bind;
            add.push_back(chrbag);
        }
    }

    CExtCharBag *Bag = Player::GetBag(m_player);
    if (!CExtCharBag::RemoveItem(Bag, &itemSubmit, ITEM_CHANGE_REASON::ICR_TASK_SUBMIT))
        return 10002;

    CExtCharBag *Bag2 = Player::GetBag(m_player);
    if (!CExtCharBag::AddItem(Bag2, &add, ITEM_CHANGE_REASON::ICR_TASK_REWARD))
        return 10002;

    float Rate = GetTaskRewardRate((TaskType)cfgTask->type);

    if (cfgTask->award_money > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_BIND_MONEY, (int32_t)((float)(nTimes * cfgTask->award_money) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->award_exp > 0)
    {
        int64_t award_exp = nTimes * cfgTask->award_exp;
        Player::addExp(m_player, (int32_t)((float)(int32_t)award_exp * Rate), 0, 1);
    }
    if (cfgTask->dilong > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_VIGOUR, (int32_t)((float)(nTimes * cfgTask->dilong) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->fuwen > 0)
    {
        int32_t v9 = (int32_t)((float)(nTimes * cfgTask->fuwen) * Rate);
        CFaBao *PlayerFaBao = Player::GetPlayerFaBao(m_player);
        CFaBao::AddFaBaoRes(PlayerFaBao, 2, v9);
    }
    if (cfgTask->rongyu > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_HONOR, (int32_t)((float)(nTimes * cfgTask->rongyu) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->acScore > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_AC_SOCRE, (int32_t)((float)(nTimes * cfgTask->acScore) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->gold > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_CASH, (int32_t)((float)(nTimes * cfgTask->gold) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->dust > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_DUST, (int32_t)((float)(nTimes * cfgTask->dust) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);
    if (cfgTask->BossScore > 0)
        Player::AddCurrency(m_player, CURRENCY_TYPE::CURRENCY_BOSS_SCORE, (int32_t)((float)(nTimes * cfgTask->BossScore) * Rate), CURRENCY_CHANGE_REASON::MCR_TASK, cfgTask->id);

    if (cfgTask->type == 37 || cfgTask->type == 41)
    {
        m_tasks.erase(tid);
    }
    else
    {
        m_tasks[tid].id = tid;
        m_tasks[tid].state = 5;
        m_tasks[tid].monster = 0;
    }

    double bnfRatio = Player::benefitRatio(m_player);
    sendTaskSubmitted(tid, (int)((double)cfgTask->award_exp * bnfRatio));

    for (auto mit = m_taskMonsters.begin(); mit != m_taskMonsters.end(); ++mit)
    {
        if (mit->tid == tid)
        {
            m_taskMonsters.erase(mit);
            sendTaskKilledMonster();
            break;
        }
    }

    Player::updateLastTaskId(m_player, tid);
    CFunctionOpen *PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_player);
    CFunctionOpen::CheckFunctionOpne(PlayerFunctionOpen, tid, 0);

    if (cfgTask->type == 1)
    {
        if (cfgTask->posttask > 0)
            receive(cfgTask->posttask);
    }

    if (cfgTask->type == 37)
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(m_player);
        CExtOperateLimit::AddLimitCount(pLimit, 2019, 1);
        CExtCharFamily *CharFamily = Player::GetCharFamily(m_player);
        CExtCharFamily::SendAddFamilyTaskCount(CharFamily);
        int32_t GongXian = nTimes * cfgTask->GongXian;
        CExtCharFamily *v24 = Player::GetCharFamily(m_player);
        CExtCharFamily::AddContribution(v24, GongXian, CURRENCY_CHANGE_REASON::FCCR_FAMILY_TASK);
    }

    if (cfgTask->type == 41)
    {
        Map *pMap = StaticObj::getMap(m_player);
        CActivityMap *pActivityMap = pMap ? dynamic_cast<CActivityMap *>(pMap) : nullptr;
        if (pActivityMap)
        {
            CActivity *Activity = CActivityMap::GetActivity(pActivityMap);
            CFamilyWar *pFamilyWar = Activity ? dynamic_cast<CFamilyWar *>(Activity) : nullptr;
            if (pFamilyWar)
                CFamilyWar::OnTaskSubmited(pFamilyWar, m_player, cfgTask->activity_score);
        }
    }

    if (cfgTask->type == 9)
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(m_player);
        CExtOperateLimit::AddLimitCount(pLimit, 2072, 1);
        int32_t Times = Player::getRecord(m_player, 2072);
        if (Times <= 4)
        {
            int32_t v31 = m_player->getLevel();
            CfgData *pCfg2 = CfgData::instance();
            int32_t TaskId = CfgData::InitXuWuTask(pCfg2, v31, Times + 1);
            if (TaskId > 0)
                receive(TaskId);
        }
    }

    if (cfgTask->type == 42)
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(m_player);
        CExtOperateLimit::AddLimitCount(pLimit, 2118, 1);
        int32_t Times = Player::getRecord(m_player, 2118);
        if (Times <= 4)
        {
            int32_t v35 = m_player->getLevel();
            CfgData *pCfg2 = CfgData::instance();
            int32_t TaskId = CfgData::InitXinMoTask(pCfg2, v35, Times + 1);
            if (TaskId > 0)
                receive(TaskId);
        }
    }

    DailyActivity *PlayerDailyActivity = Player::GetPlayerDailyActivity(m_player);
    DailyActivity::AddTaskRecord(PlayerDailyActivity, cfgTask->type);
    updateTaskTypeCount(cfgTask->type);
    return 0;
}

int32_t ChrTask::giveUp(int32_t tid)
{
    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
    if (!cfgTask)
        return 10002;

    auto it = m_tasks.find(tid);
    if (it == m_tasks.end())
        return 10002;

    if (it->second.state != 3 && it->second.state != 4 && it->second.state != 2)
        return 10002;

    if (cfgTask->type == 1 || cfgTask->type == 38)
        return 10002;

    if (cfgTask->type == 37)
    {
        if (CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(m_player), 2019) > 0)
        {
            CExtOperateLimit *pLimit = Player::GetOperateLimit(m_player);
            CExtOperateLimit::AddLimitCount(pLimit, 2019, -1);
        }
    }

    CExtCharBag *Bag = Player::GetBag(m_player);
    CExtCharBag::RemoveItem(Bag, &cfgTask->items_receive, ITEM_CHANGE_REASON::ICR_TASK_GIVEUP);
    m_tasks.erase(tid);

    for (auto mit = m_taskMonsters.begin(); mit != m_taskMonsters.end(); ++mit)
    {
        if (mit->tid == tid)
        {
            m_taskMonsters.erase(mit);
            sendTaskKilledMonster();
            break;
        }
    }

    sendTaskGiveUped(tid);
    updateTaskTypeCount(cfgTask->type);
    return 0;
}

void ChrTask::checkTaskCanSubmit(int32_t dungenID, int8_t dungeonType)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3 || pair.second.state == 4)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->condition == 10 && cfgTask->request.param1 == dungenID)
            {
                pair.second.state = 4;
            }
        }
    }
}

int32_t ChrTask::quickDone(int32_t tid)
{
    if (!m_player)
        return 10002;

    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
    if (!cfgTask)
        return 10002;

    auto it = m_tasks.find(tid);
    if (it == m_tasks.end())
        return 10002;

    if (it->second.state != 3)
        return 10002;

    if (cfgTask->quick_price <= 0)
        return 10002;

    if (!Player::DecCurrency(m_player, CURRENCY_TYPE::CURRENCY_GOLD, cfgTask->quick_price, CURRENCY_CHANGE_REASON::GCR_QUICK_TASK, tid))
        return 10002;

    it->second.state = 4;
    setTaskCanSubmit(tid, true);
    return 0;
}

bool ChrTask::receivePreCheck(int32_t tid, CfgTask *cfgTask)
{
    if (!cfgTask)
        return false;

    if (cfgTask->type == 38)
        return false;

    if (cfgTask->pretask > 0)
    {
        auto it = m_tasks.find(cfgTask->pretask);
        if (it == m_tasks.end())
            return false;
        if (it->second.state != 5 && it->second.state != 6)
            return false;
    }

    if (cfgTask->taskLimit > 0)
    {
        if (Player::getRecord(m_player, cfgTask->taskLimit) > 0)
            return false;
    }

    if (m_tasks.find(tid) != m_tasks.end())
        return false;

    if (GetTypeTask((int8_t)cfgTask->type))
        return false;

    if (cfgTask->requiredLevel > 0 && cfgTask->requiredLevel > m_player->getLevel())
        return false;

    if (cfgTask->vRequiredTask.size() > 0)
    {
        for (auto &requiredTid : cfgTask->vRequiredTask)
        {
            auto it = m_tasks.find(requiredTid);
            if (it == m_tasks.end() || it->second.state != 5)
                return false;
        }
    }

    return true;
}

bool ChrTask::submitPreCheck(int32_t tid, CfgTask *cfgTask)
{
    if (!cfgTask)
        return false;

    auto it = m_tasks.find(tid);
    if (it == m_tasks.end())
        return false;

    if (it->second.state != 4)
        return false;

    if (!setTaskCanSubmit(tid, true))
        return false;

    if (cfgTask->need_item > 0)
    {
        if (!CExtCharBag::HasItem(Player::GetBag(m_player), cfgTask->need_item, 1))
            return false;
    }

    return true;
}

void ChrTask::updateTaskUseItem(int32_t id, int32_t Count)
{
    if (!m_player)
        return;

    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->condition == 7 && cfgTask->request.param1 == id)
            {
                int32_t nCount = std::min(Count, cfgTask->request.param2);
                pair.second.monster += nCount;
                setTaskCanSubmit(pair.first, pair.second.monster >= cfgTask->request.param2);
                sendTaskKilledMonster();
            }
        }
    }
}

void ChrTask::updateTaskTypeCount(int32_t taskType)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->type == taskType)
            {
                pair.second.monster++;
                setTaskCanSubmit(pair.first, true);
                sendTaskKilledMonster();
            }
        }
    }
}

void ChrTask::updateTaskPlant(int32_t id)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->condition == 6 && cfgTask->request.param1 == id)
            {
                pair.second.monster++;
                setTaskCanSubmit(pair.first, pair.second.monster >= cfgTask->request.param2);
                sendTaskKilledMonster();
            }
        }
    }
}

void ChrTask::updateTaskMonster(int32_t Mid, int32_t group_id, int32_t level)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->condition == 1)
            {
                if (cfgTask->request.param1 == Mid || (cfgTask->request.param1 == 0 && cfgTask->request.param2 == group_id))
                {
                    pair.second.monster++;
                    setTaskCanSubmit(pair.first, pair.second.monster >= cfgTask->request.param3);
                    sendTaskKilledMonster();
                }
            }
        }
    }
}

void ChrTask::updateTaskSkill(int32_t skillID)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->condition == 9 && cfgTask->request.param1 == skillID)
            {
                pair.second.monster++;
                setTaskCanSubmit(pair.first, pair.second.monster >= cfgTask->request.param2);
                sendTaskKilledMonster();
            }
        }
    }
}

void ChrTask::updateTaskCount(int32_t Condition, int32_t param)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->condition == Condition)
            {
                if (param == 0 || cfgTask->request.param1 == param)
                {
                    pair.second.monster++;
                    setTaskCanSubmit(pair.first, pair.second.monster >= cfgTask->request.param2);
                    sendTaskKilledMonster();
                }
            }
        }
    }
}

void ChrTask::updateTaskDrop(int32_t tid, int32_t itemid)
{
    auto it = m_tasks.find(tid);
    if (it == m_tasks.end())
        return;

    if (it->second.state == 3)
    {
        CfgData *pCfg = CfgData::instance();
        CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
        if (cfgTask && cfgTask->condition == 15)
        {
            it->second.monster++;
            setTaskCanSubmit(tid, it->second.monster >= cfgTask->request.param1);
            sendTaskKilledMonster();
        }
    }
}

void ChrTask::cmdResetTrunkTask(int32_t trunkTaskId)
{
    m_trunkTaskId = trunkTaskId;
    m_tasks.clear();
    m_taskMonsters.clear();
}

bool ChrTask::isDoingTaskType(int32_t type)
{
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->type == type)
                return true;
        }
    }
    return false;
}

bool ChrTask::isDoingTask(int32_t nTaskId)
{
    return m_tasks.find(nTaskId) != m_tasks.end();
}

int8_t ChrTask::GetTaskState(int32_t nTaskId)
{
    auto it = m_tasks.find(nTaskId);
    if (it != m_tasks.end())
        return (int8_t)it->second.state;
    return 0;
}

bool ChrTask::setTaskCanSubmit(int32_t tid, bool setCondition)
{
    if (setCondition)
    {
        auto it = m_tasks.find(tid);
        if (it != m_tasks.end())
        {
            it->second.state = 4;
            return true;
        }
    }
    return false;
}

void ChrTask::sendTaskList()
{
    if (!m_player)
        return;

    int8_t ConnId = Player::getConnId(m_player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x96);
    if (!packet)
        return;

    packet->writeInt32(m_trunkTaskId);
    int32_t nSize = (int32_t)m_tasks.size();
    packet->writeInt16((int16_t)nSize);

    for (auto &pair : m_tasks)
    {
        packet->writeInt32(pair.first);
        packet->writeInt8(pair.second.state);
        packet->writeInt32(pair.second.monster);
    }

    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void ChrTask::sendTaskKilledMonster()
{
    if (!m_player)
        return;

    int8_t ConnId = Player::getConnId(m_player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x9B);
    if (!packet)
        return;

    int32_t nSize = (int32_t)m_taskMonsters.size();
    packet->writeInt16((int16_t)nSize);

    for (auto &monster : m_taskMonsters)
    {
        packet->writeInt32(monster.condition);
        packet->writeInt32(monster.request.param1);
        packet->writeInt32(monster.request.param2);
        packet->writeInt32(monster.request.param3);
    }

    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void ChrTask::saveToDB(Answer::NetPacket *packet)
{
    packet->writeInt32(m_trunkTaskId);
    int32_t nSize = (int32_t)m_tasks.size();
    packet->writeInt32(nSize);

    for (auto &pair : m_tasks)
    {
        packet->writeInt32(pair.first);
        packet->writeInt8(pair.second.state);
        packet->writeInt32(pair.second.monster);
        packet->writeInt32(pair.second.progress);
    }
}

void ChrTask::SaveDBData(PlayerDBData *dbData)
{
    MemChrTaskVector &taskVt = dbData->taskData.taskVt;
    for (auto &pair : m_tasks)
    {
        MemChrTask stu = pair.second;
        taskVt.push_back(stu);
    }
}

int32_t ChrTask::getMonster(int32_t tid)
{
    auto it = m_tasks.find(tid);
    if (it != m_tasks.end())
        return it->second.monster;
    return 0;
}

void ChrTask::setMonster(int32_t tid)
{
    auto it = m_tasks.find(tid);
    if (it != m_tasks.end())
    {
        int32_t nMonster = it->second.monster;
        CfgData *pCfg = CfgData::instance();
        CfgTask *cfgTask = CfgData::getTask(pCfg, tid);
        if (cfgTask && nMonster >= cfgTask->request.param2)
            setTaskCanSubmit(tid, true);
        sendTaskKilledMonster();
    }
}

void ChrTask::sendTaskReceived(int32_t tid)
{
    if (!m_player)
        return;

    int8_t ConnId = Player::getConnId(m_player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x97);
    if (!packet)
        return;

    packet->writeInt32(tid);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void ChrTask::sendTaskSubmitted(int32_t tid, int32_t exp)
{
    if (!m_player)
        return;

    int8_t ConnId = Player::getConnId(m_player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x98);
    if (!packet)
        return;

    packet->writeInt32(tid);
    packet->writeInt32(exp);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void ChrTask::sendTaskGiveUped(int32_t tid)
{
    if (!m_player)
        return;

    int8_t ConnId = Player::getConnId(m_player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x99);
    if (!packet)
        return;

    packet->writeInt32(tid);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

int32_t ChrTask::GetTypeTask(int8_t Type)
{
    int32_t nCount = 0;
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3 || pair.second.state == 4)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->type == Type)
                nCount++;
        }
    }
    return nCount;
}

int8_t ChrTask::GetTaskType(int32_t TaskId)
{
    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, TaskId);
    if (cfgTask)
        return (int8_t)cfgTask->type;
    return 0;
}

void ChrTask::CleanTaskType(int8_t nType)
{
    std::vector<int32_t> vErase;
    for (auto &pair : m_tasks)
    {
        if (pair.second.state == 3 || pair.second.state == 4)
        {
            CfgData *pCfg = CfgData::instance();
            CfgTask *cfgTask = CfgData::getTask(pCfg, pair.first);
            if (cfgTask && cfgTask->type == nType)
                vErase.push_back(pair.first);
        }
    }

    for (auto &id : vErase)
    {
        m_tasks.erase(id);
        for (auto mit = m_taskMonsters.begin(); mit != m_taskMonsters.end(); ++mit)
        {
            if (mit->tid == id)
            {
                m_taskMonsters.erase(mit);
                break;
            }
        }
    }

    if (!vErase.empty())
        sendTaskKilledMonster();
}

TaskRequest ChrTask::GetTaskCfgRequest(int32_t Tid)
{
    TaskRequest request;
    memset(&request, 0, sizeof(request));

    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, Tid);
    if (cfgTask)
    {
        request.param1 = cfgTask->request.param1;
        request.param2 = cfgTask->request.param2;
        request.param3 = cfgTask->request.param3;
    }
    return request;
}

void ChrTask::checkTaskDrop(int32_t nTid, int32_t nCount)
{
    auto it = m_tasks.find(nTid);
    if (it == m_tasks.end())
        return;

    if (it->second.state != 3)
        return;

    CfgData *pCfg = CfgData::instance();
    CfgTask *cfgTask = CfgData::getTask(pCfg, nTid);
    if (!cfgTask || cfgTask->condition != 15)
        return;

    it->second.monster += nCount;
    setTaskCanSubmit(nTid, it->second.monster >= cfgTask->request.param1);
    sendTaskKilledMonster();
}

void ChrTask::dropItemFromGroup(DropItem &retstr, int32_t groupId)
{
    memset(&retstr, 0, sizeof(retstr));
}

float ChrTask::GetTaskRewardRate(TaskType type)
{
    return 1.0f;
}
