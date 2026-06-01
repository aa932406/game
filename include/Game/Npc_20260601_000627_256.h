#ifndef _NPC_H_
#define _NPC_H_

#include "Common/CommonTypes.h"
#include "Config/CfgNpc.h"
#include "Game/Unit.h"
#include "Utility/Position.h"
#include <string>

// 前置声明
class Player;
class Dungeon;
class Answer_NetPacket;

namespace Answer {
    class NetPacket;
}

/**
 * @brief Npc - 非玩家角色，继承自 Unit
 */
class Npc : public Unit
{
public:
    Npc();
    virtual ~Npc();

    // 重置 NPC
    void reset();

    // 检查是否生命周期结束
    bool isLifeEnd() const;

    // 玩家对话回调
    void onTalkByPlayer(Player* player);

    // 初始化
    void init(const CfgNpc& cfgNpc, int32_t endTime);

    // 打包附加信息到网络包
    bool appendInfo(Answer::NetPacket* packet);

    // 获取摊位位置
    Position GetStallPos() const;

    // 检查平台兼容性
    bool checkPlatform(const std::string& platform) const;

    // 获取副本 NPC 消耗配置
    CfgDungeonNpcCost GetNpcCost(int8_t choice) const;

protected:
    CfgNpc  m_cfgNpc;       // NPC 配置
    int64_t m_endTime;      // 结束时间戳
    int32_t m_nOwner;       // 所属者 ID
};

#endif // _NPC_H_
