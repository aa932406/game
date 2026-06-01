#ifndef _CFGCFGCARRIERTABLE_H_
#define _CFGCFGCARRIERTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgCarrier.h"
#include "Config/CfgCarrierAttr.h"


/**
 * @brief CfgCarrierTable - Config data class
 */
class CfgCarrierTable
{
public:
    CfgCarrierTable();
    ~CfgCarrierTable();

    void AddCarrier(const CfgCarrier& stu);
    void AddCarrierAttr(const CfgCarrierAttr& stu);

public:
    std::map<int, CfgCarrier> m_mCarrier;
    std::map<int, CfgCarrierAttr> m_mCarrierAttr;
};

#endif // _CFGCFGCARRIERTABLE_H_
