#ifndef _OTHER_MEMCHRBAG_H_
#define _OTHER_MEMCHRBAG_H_
#include <cstdint>
#ifndef MEMCHRBAG_DEFINED
#define MEMCHRBAG_DEFINED
struct MemChrBag {
    int32_t itemId;
    int8_t  itemClass;
    int32_t itemCount;
    int64_t srcId;
    int8_t  bind;
    int32_t endTime;
    int32_t& slot() { return itemId; }
    const int32_t& slot() const { return itemId; }
};
#endif
#endif
