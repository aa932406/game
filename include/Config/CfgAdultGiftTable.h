#ifndef _CFGADULTGIFTTABLE_H_
#define _CFGADULTGIFTTABLE_H_

#include "Common/CommonTypes.h"
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


/**
 * @brief CfgAdultGiftTable - Config data class
 */
class CfgAdultGiftTable
{
public:

    void Add(CfgAdultGiftTable * this, CfgAdultGift * stu);

private:
        std::map<int32_t, CfgAdultGift*> m_mAdult;
};

#endif // _CFGADULTGIFTTABLE_H_