#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <cstdint>

namespace Answer {

class Random {
public:
    static Random* GetInstance();
    int32_t generate(int32_t min, int32_t max);
    float generateFloat();
};

}

#endif
