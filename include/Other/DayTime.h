#ifndef _DAYTIME_H_
#define _DAYTIME_H_
#include <cstdint>

class DayTime {
public:
    static int32_t now();
    static int32_t dayzero(int32_t t);
    static int32_t getMonthStart();
};

#endif
