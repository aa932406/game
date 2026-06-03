#ifndef _FESTIVALRANK_H_
#define _FESTIVALRANK_H_

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

class FestivalRank
{
public:
    FestivalRank();
    FestivalRank(const FestivalRank &a2);
    ~FestivalRank();

    void CleanUp();

    int32_t nIndex;
    std::string strDB;
    int32_t nCharId;
    std::string strName;
    char szName[64];
    int32_t nScore;
    int32_t nValue;
    int32_t nTime;
    int8_t bChange;
};

#endif // _FESTIVALRANK_H_
