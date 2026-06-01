#ifndef _CFGTRAILER_H_
#define _CFGTRAILER_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgTrailer - Config data class
 */
class CfgTrailer
{
public:
    int32_t nId;
    std::list<AddAttribute> nAttrList;
    int32_t TrailerQuality;
    int32_t nTaskId;
    int32_t PureDamage;

    CfgTrailer() = default;
    CfgTrailer(const CfgTrailer& a2);
    ~CfgTrailer() = default;
    CfgTrailer& operator=(const CfgTrailer& a2);
};

#endif // _CFGTRAILER_H_
