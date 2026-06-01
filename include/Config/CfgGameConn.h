#ifndef _CFGCFGGAMECONN_H_
#define _CFGCFGGAMECONN_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgGameConn - Config data class
 */
class CfgGameConn
{
public:
    std::string host;
    int32_t port;

    CfgGameConn() = default;
    ~CfgGameConn() = default;
    CfgGameConn(const CfgGameConn& a2);
    CfgGameConn& operator=(const CfgGameConn& a2);
};

#endif // _CFGCFGGAMECONN_H_
