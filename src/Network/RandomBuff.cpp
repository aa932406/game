#include "Network/RandomBuff.h"

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include "Activity/CActivity.h"
#include "Activity/CActivityMap.h"
#include "Config/CfgData.h"
#include "Game/Player.h"
#include "Map/Map.h"
#include "Map/MapManager.h"
#include "Other/CPeerlessWar.h"

// ============================================================
// 构造 / 析构
// ============================================================

RandomBuff::RandomBuff()
    : ItemEffect()
    , m_nSumRate(0)
{
}

RandomBuff::~RandomBuff()
{
}

// ============================================================
// 效果执行 - 在 CPeerlessWar 活动中随机选取一个 buff 添加
// ============================================================

int32_t RandomBuff::effect(Player* launcher, Unit* /*target*/, int32_t* /*count*/)
{
    if (!launcher)
        return 10002;

    Map* pMap = launcher->getMap();
    if (!pMap)
        return 10002;

    CActivityMap* pActMap = dynamic_cast<CActivityMap*>(pMap);
    if (!pActMap)
        return 10002;

    CActivity* pActivity = pActMap->GetActivity();
    if (!pActivity)
        return 10002;

    CPeerlessWar* pWar = dynamic_cast<CPeerlessWar*>(pActivity);
    if (!pWar || !pWar->IsRuning())
        return 10002;

    // 没有配置任何 buff，直接返回
    if (m_nSumRate <= 0 || m_buffs.empty())
        return 10002;

    // 根据概率随机选择一个 buff
    Answer::Random* pRandom = Answer::Singleton<Answer::Random>::instance();
    int32_t nRand = pRandom->generate(0, m_nSumRate);

    for (auto it = m_buffs.begin(); it != m_buffs.end(); ++it)
    {
        if (it->nRate >= nRand && it->m_cfgBuff)
        {
            pWar->AddActivityBuff(launcher, it->m_cfgBuff->id, 1);
            return 0;
        }
    }

    return 10002;
}

// ============================================================
// 解析效果配置 - 格式: "buffId:rate|buffId:rate|..."
// ============================================================

bool RandomBuff::parseEffect(int32_t /*id*/, const std::string* strEffect)
{
    if (!strEffect || strEffect->empty())
        return false;

    m_nSumRate = 0;
    m_buffs.clear();

    // 按 "|" 分割各个 buff 配置
    std::vector<std::string> strParams;
    std::istringstream stream(*strEffect);
    std::string token;
    while (std::getline(stream, token, '|'))
    {
        if (!token.empty())
            strParams.push_back(token);
    }

    for (size_t i = 0; i < strParams.size(); ++i)
    {
        // 每个条目格式: "buffId:rate"
        size_t colonPos = strParams[i].find(':');
        if (colonPos == std::string::npos)
            continue;

        int32_t buffId = std::atoi(strParams[i].substr(0, colonPos).c_str());
        int32_t rate   = std::atoi(strParams[i].substr(colonPos + 1).c_str());

        CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
        CfgBuff* pBuff = pCfgData->getBuff(buffId);
        if (!pBuff)
            continue;

        m_nSumRate += rate;

        randBuff buff;
        buff.nRate     = m_nSumRate;
        buff.m_cfgBuff = pBuff;
        m_buffs.push_back(buff);
    }

    return true;
}
