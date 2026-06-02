#ifndef _CFGADULTGIFTTABLE_H_
#define _CFGADULTGIFTTABLE_H_

#include "Common/CommonTypes.h"
#include "Config/CfgAdultGift.h"
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>


/**
 * @brief CfgAdultGiftTable - Config data class
 */
class CfgAdultGiftTable
{
public:

    void Add(CfgAdultGift* stu);

private:
        std::map<int32_t, CfgAdultGift*> m_mAdult;
};

#endif // _CFGADULTGIFTTABLE_H_