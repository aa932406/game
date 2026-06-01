#include "Config/CfgGameConn.h"

CfgGameConn::CfgGameConn(const CfgGameConn& a2)
{
    host = a2.host;
    port = a2.port;
}

CfgGameConn& CfgGameConn::operator=(const CfgGameConn& a2)
{
    if (this != &a2)
    {
        host = a2.host;
        port = a2.port;
    }
    return *this;
}
