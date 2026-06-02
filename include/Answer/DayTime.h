#ifndef _ANSWER_DAYTIME_H_
#define _ANSWER_DAYTIME_H_

#include "../Other/DayTime.h"
#include <cstdint>
#include <string>

namespace Answer {
    class DayTime : public ::DayTime {
    public:
        static int32_t StringToIntTime(const std::string* str);
    };
}

#endif
