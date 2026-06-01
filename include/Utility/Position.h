#ifndef _POSITION_H_
#define _POSITION_H_

#include "Common/CommonTypes.h"
#include <cmath>

// Position class for tile/pixel math (distinct from struct Position in CfgData.h)
class Position
{
public:
    Position();
    Position(int32_t x_, int32_t y_);

    int32_t tileDistance(const Position* pos) const;
    double distance(const Position* pos) const;

    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Position& other) const { return !(*this == other); }

    // 成员变量
    int32_t x;
    int32_t y;
};

#endif // _POSITION_H_