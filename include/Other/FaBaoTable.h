#ifndef _FABAOTABLE_H_
#define _FABAOTABLE_H_

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
class CfgFaBao;

class FaBaoTable
{
public:
    FaBaoTable();
    ~FaBaoTable();

    void AddFaBao(int8_t Type, CfgFaBao * Stu);
    const CfgFaBao* GetFaBaoCfg(int8_t Type, int32_t Level);

private:
    std::map<int, CfgFaBao> m_CfgFaBao[4];
};

#endif // _FABAOTABLE_H_
