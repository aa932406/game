#ifndef _TREASUREMAPTABALE_H_
#define _TREASUREMAPTABALE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
class TreasureMapCfg;
struct TreasureMapEventCfg;
struct MapPos;

class TreasureMapTabale
{
public:
    TreasureMapTabale();
    ~TreasureMapTabale();

    void AddTreasureMapCfg(TreasureMapCfg *p_stu);
    void AddTreasureMapEventCfg(TreasureMapEventCfg *p_stu);
    const MapPos* GetMapPos(int32_t ItemId);
    const TreasureMapEventCfg* GetEvent(int32_t ItemId);

private:
    std::map<int, TreasureMapCfg> m_TreasureMapCfgMap;
    std::map<int, TreasureMapEventCfg> m_TreasureMapEventCfgMap;
};

#endif // _TREASUREMAPTABALE_H_
