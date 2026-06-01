#ifndef _VIPTABLE_H_
#define _VIPTABLE_H_

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
class VipCfg;

class VipTable
{
public:
    VipTable();

    void InitVipTable();
    const VipCfg* GetVipCfg(int8_t VipLevel);
    int8_t GetVipLevel(int32_t VipExp);

private:
    std::map<int8_t, VipCfg> m_VipCfgMap;
};

#endif // _VIPTABLE_H_
