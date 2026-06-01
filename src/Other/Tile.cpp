#include "Other/Tile.h"

Tile::Tile()
    : m_parent(0)
    , m_x(0)
    , m_y(0)
    , m_depth(0)
    , m_mask(1)
{
}

Position Tile::getPosition() const
{
    return Position(m_x, m_y);
}

bool Tile::isWalkable() const
{
    return !m_mask || m_mask == 2;
}

void Tile::init(int32_t parent, int32_t x, int32_t y, int32_t depth, int32_t mask)
{
    m_parent = parent;
    m_x = x;
    m_y = y;
    m_depth = depth;
    m_mask = mask;
}
