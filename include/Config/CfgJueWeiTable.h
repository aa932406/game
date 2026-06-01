#ifndef _CFGCFGJUEWEITABLE_H_
#define _CFGCFGJUEWEITABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgJueWei.h"


/**
 * @brief CfgJueWeiTable - Config data class
 */
class CfgJueWeiTable
{
public:
    CfgJueWeiTable();
    ~CfgJueWeiTable();

    bool AddJueWei(const CfgJueWei& stu);

public:
    std::map<int8_t, CfgJueWei> m_mJueWei;
};

#endif // _CFGCFGJUEWEITABLE_H_
