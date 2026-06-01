#ifndef _DANTIANCFG_H_
#define _DANTIANCFG_H_

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
class Param2;

class DanTianCfg
{
public:
    DanTianCfg();
    DanTianCfg(const DanTianCfg &a2);
    ~DanTianCfg();

    int32_t RandHighDanTianId();
    int32_t RandDanTianId();

    int32_t nMid;
    std::list<Param2> nItemRate;
    int32_t nMaxRate;
    int32_t nTime;
    int32_t nRate;
    std::list<Param2> nHighItemRate;
    int32_t nHighMaxRate;
};

#endif // _DANTIANCFG_H_
