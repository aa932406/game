#ifndef _CFGCFGEXCHANGETABLE_H_
#define _CFGCFGEXCHANGETABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgExchange.h"


/**
 * @brief CfgExchangeTable - Config data class
 */
class CfgExchangeTable
{
public:
    CfgExchangeTable();
    ~CfgExchangeTable();

    void Add(const CfgExchange& stu);

public:
    std::map<int, CfgExchange> m_mExchange;
};

#endif // _CFGCFGEXCHANGETABLE_H_
