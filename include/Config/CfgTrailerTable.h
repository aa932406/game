#ifndef _CFGCFGTRAILERTABLE_H_
#define _CFGCFGTRAILERTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgTrailer.h"


/**
 * @brief CfgTrailerTable - Config data class
 */
class CfgTrailerTable
{
public:
    CfgTrailerTable();
    ~CfgTrailerTable();

    void Add(const CfgTrailer& stu);

    const CfgTrailer* GetTrailer(int32_t nId);
    bool IsTrailerExist(int32_t nId);
    int32_t GetTrailerQuality(int32_t nId);
    int32_t GetTrailerTaskId(int32_t nId);
    void Clear();

public:
    std::map<int, CfgTrailer> m_mTrailer;
};

#endif // _CFGCFGTRAILERTABLE_H_
