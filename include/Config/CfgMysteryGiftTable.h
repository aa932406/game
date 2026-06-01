#ifndef _CFGCFGMYSTERYGIFTTABLE_H_
#define _CFGCFGMYSTERYGIFTTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgMysteryGift.h"


/**
 * @brief CfgMysteryGiftTable - Config data class
 */
class CfgMysteryGiftTable
{
public:
    CfgMysteryGiftTable();
    ~CfgMysteryGiftTable();

    void Add(const CfgMysteryGift& stu);

public:
    std::map<int, CfgMysteryGift> m_mMysteryGift;
};

#endif // _CFGCFGMYSTERYGIFTTABLE_H_
