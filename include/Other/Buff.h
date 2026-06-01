#ifndef _BUFF_H_
#define _BUFF_H_

#include <cstdint>
#include "Common/CommonTypes.h"
#include "Utility/UnitHandle.h"

// 前置声明
class CfgBuff;
class Unit;

class Buff
{
public:
    Buff();
    Buff(int32_t buffId, int32_t buffLevel, Unit* target, Unit* launcher);
    ~Buff();

    // 基本信息
    int32_t getId() const { return m_buffId; }
    int32_t getLevel() const { return m_buffLevel; }
    int32_t getSpecial() const { return m_special; }
    
    // 时间相关
    int64_t remainTick() const;
    bool expire() const;
    
    // 效果相关
    void effect();
    void interval();
    void restore();
    bool effectTimes();
    bool shieldDamage(int32_t* nDamage);
    void trigTime();
    
    // 是否需要显示
    bool NeedShow() const;

private:
    int32_t m_buffId;           // BuffID
    int32_t m_buffLevel;        // Buff等级
    int32_t m_special;          // 特殊类型
    int8_t  m_index;            // 槽位索引
    
    const CfgBuff* m_cfgBuff;   // Buff配置
    
    Unit*   m_target;           // 目标单位
    Unit*   m_launcher;         // 施放者
    UnitHandle m_launcherHandle; // 施放者句柄
    
    int64_t m_endTick;          // 结束时间
    int64_t m_lastIntervalTick; // 上次间隔时间
    int64_t m_nEffectTimes;     // 效果次数
    
    int32_t m_nShield;          // 护盾值
    int32_t m_nType;            // Buff类型
    
    // 恢复数据
    std::vector<BuffRestore> m_restore;
};

#endif // _BUFF_H_