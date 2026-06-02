#ifndef _SHIZHUANGSUITCFG_H_
#define _SHIZHUANGSUITCFG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class ShiZhuangSuitInfo;

class ShiZhuangSuitCfg
{
public:
    ShiZhuangSuitCfg(const ShiZhuangSuitCfg\& a2);

    int32_t nSuidId;
    int32_t nCount;
    std::list<ShiZhuangSuitInfo> lInfo;
};

#endif // _SHIZHUANGSUITCFG_H_