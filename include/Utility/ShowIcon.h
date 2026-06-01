#ifndef _SHOWICON_H_
#define _SHOWICON_H_

#include "Common/CommonTypes.h"

// 前置声明
namespace Answer {
    class NetPacket;
}

class ShowIcon
{
public:
    int32_t nId;        // 图标ID
    int8_t  nState;     // 状态
    int32_t nLeftTime;  // 剩余时间
    int8_t  nIconLeft;  // 左图标
    int32_t nIconRight; // 右图标
    int8_t  nEffects;   // 效果

    void AppendInfo(Answer::NetPacket* packet) const;
};

#endif // _SHOWICON_H_