#include "Other/TileManager.h"

#include <cmath>

namespace Answer
{
    class Random
    {
    public:
        size_t generate(int32_t min, int32_t max);
    };

    template<typename T>
    class Singleton
    {
    public:
        static T* instance();
    };
}

class CfgData;
struct CfgMap
{
    int32_t id;
    int32_t width;
    int32_t height;
    int32_t Rdx;
    bool hide_mini;
};

TileManager::TileManager()
{
}

void TileManager::Init()
{
}

void TileManager::addMap(int32_t mapid, int32_t width, int32_t height, int32_t Rdx_0)
{
    if (mapid <= 0 || width <= 0 || height <= 0)
        return;

    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    CfgMap* pCfg = CfgData::getMap(v5, mapid);
    if (!pCfg)
        return;

    int32_t count = height * width;
    Tile* pTile = new Tile[count]();

    if (pCfg->hide_mini)
        parseRd(mapid, width, height, pTile, Rdx_0);
    else
        parseRdx(mapid, width, height, pTile, Rdx_0);

    m_allTiles[mapid] = pTile;

    for (int32_t x = 0; x < width; ++x)
    {
        for (int32_t y = 0; y < height; ++y)
        {
            if (pTile[x + width * y].isWalkable() && (x || y))
            {
                m_walkablePositions[mapid].push_back(Position(x, y));
            }
        }
    }
}

Tile* TileManager::getTiles(int32_t mapid)
{
    std::map<int, Tile*>::iterator it = m_allTiles.find(mapid);
    if (it != m_allTiles.end())
        return it->second;
    return 0;
}

Position TileManager::getRandomWalkablePosition(int32_t mapid)
{
    Position pos(-1, -1);
    std::map<int, std::vector<Position>>::iterator it = m_walkablePositions.find(mapid);
    if (it != m_walkablePositions.end())
    {
        if (!it->second.empty())
        {
            int32_t idx = static_cast<int32_t>(Answer::Singleton<Answer::Random>::instance()->generate(0, it->second.size() - 1));
            return it->second[idx];
        }
    }
    return pos;
}

void TileManager::parseRd(int32_t mapid, int32_t width, int32_t height, Tile* pTile, int32_t Rdx_0)
{
    (void)mapid;
    (void)Rdx_0;
    for (int32_t i = 0; i < width * height; ++i)
        pTile[i].m_mask = 0;
}

void TileManager::parseRdx(int32_t mapid, int32_t width, int32_t height, Tile* pTile, int32_t Rdx_0)
{
    (void)mapid;
    (void)Rdx_0;
    for (int32_t i = 0; i < width * height; ++i)
        pTile[i].m_mask = 0;
}
