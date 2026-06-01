#ifndef _CFGSYSMAIL_H_
#define _CFGSYSMAIL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief CfgSysMail - Config data class
 */
class CfgSysMail
{
public:
    int32_t id;
    std::string sender_name;
    std::string title;
    std::string content;
    std::vector<MemChrBag> item;

    CfgSysMail() = default;
    CfgSysMail(const CfgSysMail& a2);
    ~CfgSysMail() = default;
    CfgSysMail& operator=(const CfgSysMail& a2);
};

#endif // _CFGSYSMAIL_H_
