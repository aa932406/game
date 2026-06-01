#ifndef _CFGNPC_H_
#define _CFGNPC_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief CfgDungeonNpcCost - NPC dungeon cost entry
 */
struct CfgDungeonNpcCost
{
    int32_t ItemType;
    int32_t ItemId;
    int32_t ItemCount;
};

/**
 * @brief CfgNpc - Config data class
 */
class CfgNpc
{
public:
    int32_t id;
    int32_t level;
    std::vector<int> mapids;
    int32_t x;
    int32_t y;
    int32_t func;
    int32_t func_extra;
    std::vector<int> params;
    int32_t unite_flag;
    std::vector<std::string> platforms;
    std::vector<CfgDungeonNpcCost> m_vNpcCost;

    CfgNpc() = default;
    CfgNpc(const CfgNpc& a2);
    ~CfgNpc() = default;
    CfgNpc& operator=(const CfgNpc& a2);

    bool checkPlatform(const std::string& platform) const;
};

#endif // _CFGNPC_H_
