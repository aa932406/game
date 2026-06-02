#include "Other/DayTime.h"
#include "Answer/DayTime.h"

int32_t Answer::DayTime::daydiff(int32_t t) {
    return (now() - t) / 86400;
}
