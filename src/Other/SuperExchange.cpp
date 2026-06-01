#include "Other/SuperExchange.h"
#include "Common/Common.h"
#include "Game/Player.h"
#include <cstdlib>

SuperExchange::SuperExchange() : m_id(0) {}
SuperExchange::~SuperExchange() {}
bool SuperExchange::parseEffect(int32_t id, const std::string* strEffect)
{
    m_id = id;
    StringVector params;
    Common::splistString(strEffect->c_str(), &params, "|");
    if (params.empty())
        return false;
    for (size_t i = 0; i < params.size(); ++i)
    {
        StringVector subParams;
        Common::splistString(params[i].c_str(), &subParams, ":");
        if (subParams.size() == 2)
        {
            SuperExchangeToItem toItem;
            toItem.type = atoi(subParams[0].c_str());
            toItem.value = atoi(subParams[1].c_str());
            m_exchangeToItems.push_back(toItem);
        }
        StringVector().swap(subParams);
    }
    return !m_exchangeToItems.empty();
}
int32_t SuperExchange::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (m_exchangeToItems.empty())
        return 10002;
    if (!*count)
        *count = 1;
    for (size_t i = 0; i < m_exchangeToItems.size(); ++i)
    {
        const SuperExchangeToItem& toItem = m_exchangeToItems[i];
        if (toItem.type == 1)
            Player::addExp(launcher, toItem.value * *count, 0, 1);
        else if (toItem.type == 2)
            Player::AddCurrency(launcher, CURRENCY_TYPE::CURRENCY_MONEY, toItem.value * *count, CURRENCY_CHANGE_REASON::MCR_SUPER_EXCHANGE, m_id);
        else if (toItem.type == 7)
            Player::AddCurrency(launcher, CURRENCY_TYPE::CURRENCY_CASH, toItem.value * *count, CURRENCY_CHANGE_REASON::GCR_SUPER_EXCHANGE, 0);
    }
    return 0;
}
