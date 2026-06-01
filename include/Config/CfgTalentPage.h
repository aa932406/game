#ifndef _CFGTALENTPAGE_H_
#define _CFGTALENTPAGE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgTalentPage - Config data class
 */
class CfgTalentPage
{
public:
    int32_t job;
    std::list<int> talents;

    CfgTalentPage() = default;
    CfgTalentPage(const CfgTalentPage& a2);
    ~CfgTalentPage() = default;
    CfgTalentPage& operator=(const CfgTalentPage& a2);
};

#endif // _CFGTALENTPAGE_H_
