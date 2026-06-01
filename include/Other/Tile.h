#ifndef _TILE_H_
#define _TILE_H_

#include <cstdint>

#include "Utility/Position.h"

class Tile
{
public:
    Tile();

    Position getPosition() const;
    bool isWalkable() const;
    void init(int32_t parent, int32_t x, int32_t y, int32_t depth, int32_t mask);

    int32_t m_parent;
    int32_t m_x;
    int32_t m_y;
    int32_t m_depth;
    int32_t m_mask;
};

#endif // _TILE_H_
