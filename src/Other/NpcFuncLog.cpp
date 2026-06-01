#include "Other/NpcFuncLog.h"

NpcFuncLog::NpcFuncLog()
    : nCid(0)
    , nFuncId(0)
    , nMapId(0)
    , nX(0)
    , nY(0)
    , nTime(0)
{
}

NpcFuncLog::NpcFuncLog(const NpcFuncLog& a2)
    : nCid(a2.nCid)
    , sName(a2.sName)
    , nFuncId(a2.nFuncId)
    , nMapId(a2.nMapId)
    , nX(a2.nX)
    , nY(a2.nY)
    , nTime(a2.nTime)
{
}
