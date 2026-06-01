#ifndef _CFLOPDRAW_H_
#define _CFLOPDRAW_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

typedef std::map<int32_t, int32_t> Int32Int32Map;
typedef std::map<int32_t, Int32Int32Map> Int32Int32Int32Map;

class CFlopDraw
{
public:
    CFlopDraw();
    ~CFlopDraw();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    Int32Int32Int32Map m_FlopDrawRecordMap;
};

#endif
