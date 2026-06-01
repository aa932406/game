#include "Utility/Position.h"
#include <cmath>
#include <algorithm>

Position::Position()
    : x(0), y(0)
{
}

Position::Position(int32_t x_, int32_t y_)
    : x(x_), y(y_)
{
}

int32_t Position::tileDistance(const Position* pos) const
{
    int dy = std::abs(y - pos->y);
    int dx = std::abs(x - pos->x);
    return std::max(dx, dy);
}

double Position::distance(const Position* pos) const
{
    double dx = static_cast<double>(pos->x - x);
    double dy = static_cast<double>(pos->y - y);
    return std::sqrt(dx * dx + dy * dy);
}
