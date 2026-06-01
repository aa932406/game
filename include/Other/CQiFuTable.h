#ifndef _CQIFUTABLE_H_
#define _CQIFUTABLE_H_

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
struct QiFuCfg;

class CQiFuTable
{
public:
    CQiFuTable();
    ~CQiFuTable();

    void InitQiFuTable();
    void GetQiFuCfg(int8_t QiFuType, int32_t nLevel, int8_t QiFuTimes);

private:
    std::map<int, std::map<signed char, QiFuCfg>> m_QiFuMoneyCfg;
    std::map<int, std::map<signed char, QiFuCfg>> m_QiFuExpCfg;
};

#endif // _CQIFUTABLE_H_
