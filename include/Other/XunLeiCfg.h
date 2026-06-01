#ifndef _XUNLEICFG_H_
#define _XUNLEICFG_H_

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

struct MemChrBag;

class XunLeiCfg
{
public:
    XunLeiCfg(XunLeiCfg *const this, const XunLeiCfg *const a2);

    int32_t nIndex;
    int32_t nType;
    std::vector<MemChrBag> vRewards;
    int32_t nCondition;
};

#endif // _XUNLEICFG_H_