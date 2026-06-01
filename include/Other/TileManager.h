#ifndef _TILEMANAGER_H_
#define _TILEMANAGER_H_

#include <cstdint>
#include <map>
#include <vector>

#include "Other/Tile.h"
#include "Utility/Position.h"

class TileManager
{
public:
    TileManager();

    void Init();
    void addMap(int32_t mapid, int32_t width, int32_t height, int32_t Rdx_0);
    Tile* getTiles(int32_t mapid);
    Position getRandomWalkablePosition(int32_t mapid);

private:
    void parseRd(int32_t mapid, int32_t width, int32_t height, Tile* pTile, int32_t Rdx_0);
    void parseRdx(int32_t mapid, int32_t width, int32_t height, Tile* pTile, int32_t Rdx_0);

    std::map<int, Tile*> m_allTiles;
    std::map<int, std::vector<Position>> m_walkablePositions;
};

#endif // _TILEMANAGER_H_
