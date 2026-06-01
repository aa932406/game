#include "Other/PathFinder.h"

#include <cstring>
#include <algorithm>

int32_t PathFinder::gValue(Node *pSrc, Position dest)
{
    if (!pSrc) return 0;
    
    if (pSrc->pos.y == dest.y)
        return pSrc->g + 10;
    if (pSrc->pos.y + 2 == dest.y || pSrc->pos.y - 2 == dest.y)
        return pSrc->g + 20;
    return pSrc->g + 14;
}

int32_t PathFinder::hValue(Position src, Position dest)
{
    int32_t dx = 10 * src.x + 10 * (src.y & 1) / 2 - (10 * dest.x + 10 * (dest.y & 1) / 2);
    int32_t dy = 10 * (src.y - dest.y);
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

