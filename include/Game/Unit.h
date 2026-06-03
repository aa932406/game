#ifndef _UNIT_H_
#define _UNIT_H_

#include "Common/CommonTypes.h"
#include "Utility/Position.h"
#include "Other/StaticObj.h"
#include "Other/CObjAttrs.h"
#include "Other/CObjState.h"
#include <cmath>

// Buff 前置声明
class Buff;

// 最大Buff数量
#ifndef MAX_BUFF_COUNT
#define MAX_BUFF_COUNT 40
#endif

// Unit 继承自 StaticObj (从反编译代码确认: StaticObj::StaticObj(this, entitytype))
class Unit : public StaticObj
{
public:
    Unit(EntityType entitytype);
    virtual ~Unit();

    // 获取单位句柄
    UnitHandle getHandle() const;
    
    // 重置单位
    void reset();
    
    // 更新单位
    virtual void update();
    
    // 检查HP回复
    void checkHPRef();
    
    // 离开格子
    void leaveTile();
    
    // 设置在格子位置
    void setAtTile(Map* pMap, int32_t tx, int32_t ty);
    
    // 设置目标格子
    void setTargetTile(int32_t tx, int32_t ty);
    
    // 重置方向
    bool resetDirection(int32_t tx, int32_t ty);
    
    // 瞬移
    void instantMove(int32_t tx, int32_t ty, InstanceMoveReason reason, int8_t dir);
    
    // 设置站立
    void setStand();
    
    // 判断是否在矩形区域内
    bool isInRectangle(Position pst1, Position pst2) const;
    
    // 更新位置
    bool updatePosition();
    
    // 是否正在移动
    bool isMoving() const;
    
    // 获取tick
    int64_t getTick() const;
    
    // 获取当前时间
    int32_t getNow() const;
    
    // 静态包装方法
    static int64_t getTick(Unit* unit) { return unit ? unit->getTick() : 0; }
    static int32_t getNow(Unit* unit) { return unit ? unit->getNow() : 0; }
    static void removeBuff(Unit* unit, int32_t buffId) { if (unit) unit->removeBuff(buffId); }
    
    // 获取本地时间
    void getLocalNow(struct tm* retstr) const;
    
    // 像素距离
    double pixelDistance(Unit* unit) const;
    
    // 格子距离
    int32_t tileDistance(Unit* unit) const;
    
    // 是否存活
    virtual bool isAlive() const;
    
    // 护盾伤害
    int32_t shieldDamage(int32_t damage);
    
    // 属性相关
    int32_t GetBaseAttr(CObjAttrs::Index_T nIdx) const;
    int32_t GetAttrValue(CObjAttrs::Index_T nIdx) const;
    int32_t AddAttrValue(CObjAttrs::Index_T nIdx, int32_t nAddVal);
    void SetAttrValue(CObjAttrs::Index_T nIdx, int32_t nVal);
    int32_t GetBuffAttrRatio(CObjAttrs::Index_T nIdx) const;
    int32_t AddBuffAttrRatio(CObjAttrs::Index_T nIdx, int32_t nRate);
    int32_t GetBuffAttrValue(CObjAttrs::Index_T nIdx) const;
    int32_t AddBuffAttrValue(CObjAttrs::Index_T nIdx, int32_t nAddVal);
    
    // 速度Buff
    int32_t AddSpeedBuffRatio(int32_t nRatio);
    int32_t RemoveSpeedBuffRatio(int32_t nRatio);
    int32_t GetSpeedBuffRatio() const;
    
    // Buff状态
    bool HasBuffState(CObjState::Index_T nIdx) const;
    void AddBuffState(CObjState::Index_T nIdx);
    void RemoveBuffState(CObjState::Index_T nIdx);
    
    // HP/MP相关
    int64_t GetMaxHP() const;
    int32_t GetViewHP() const;
    int32_t GetViewMaxHP() const;
    void AddHP(int32_t nAddValue);
    void AddKunLi(int32_t nAddValue);
    int64_t GetMaxKunLi() const;
    void AddMP(int32_t nAddValue);
    void FillHP(int32_t nPercent);
    void FillMP(int32_t nPercent);
    
    // 技能标记
    int32_t GetSkillFlag(bool bIncrease);
    
    // 属性调整
    void resetUnitAttr();
    void adjustUnitAttr();
    void adjustAttr(int32_t* nAttr, int32_t nMaxValue, int32_t nMinValue);
    
    // Buff相关
    bool HasBuffById(int32_t buffId) const;
    int32_t getBuffCount() const;
    int32_t addBuff(Buff* buff);
    void removeBuff(int32_t buffId);
    void removeBuffBySpecail(int32_t special);
    void checkBuffEffect(BUFF_EFFECT_TYPE type);
    void clearDebuff(int32_t nCount);
    void clearBuff(int32_t nCount);
    void clearContrlBuff();
    Buff* findBuffBySpecail(int32_t special);
    bool checkBuffBySpecail(int32_t special) const;
    void getItemBuffs(MemChrBuffVector* retstr) const;
    void checkBuffList();
    
    // 设置同步标志
    void setNeedSync();
    
    // 同步和广播
    void syncBasicData();
    void broadcastMove();
    void broadcastMoveMonster(Position Pos);
    void broadcastKickOutedMove();
    void onThrowed();
    void broadcastThrowed();
    void broadcastLeave();
    void broadcastBasicData();
    void broadcastSkillChant(int32_t nSkillId, int32_t nChantTime);
    void broadcastBreakSkillChant();
    void packageBuffList(Answer::NetPacket* packet) const;
    void broadcastAddBuff(int8_t nIndex, Buff* pBuff);
    void broadcastBuffEnd(int8_t nIndex, int32_t nReason);
    void broadcastBuffEnd(const Int8List* buffs, int32_t nReason);
    void deathClearBuff();
    
    // 方向速度
    double getDirectionSpeed() const;
    
    // 方向相关
    Direction getDirection() const { return static_cast<Direction>(m_direction); }
    void setDirection(Direction dir) { m_direction = static_cast<int8_t>(dir); }
    Direction getDirectionTo(const Unit* target) const {
        if (!target) return Direction::DOWN;
        int32_t dx = target->m_currentTile.x - m_currentTile.x;
        int32_t dy = target->m_currentTile.y - m_currentTile.y;
        if (dx == 0 && dy >= 0) return Direction::DOWN;
        if (dx == 0 && dy < 0) return Direction::UP;
        if (dy == 0 && dx >= 0) return Direction::RIGHT;
        if (dy == 0 && dx < 0) return Direction::LEFT;
        static constexpr double kPi = 3.141592653589793;
        double angle = std::atan2(static_cast<double>(-dy), static_cast<double>(dx));
        int32_t octant = static_cast<int32_t>((angle / kPi * 4.0) + 0.5) % 8;
        return static_cast<Direction>(octant);
    }
    
    // 目标格子
    Position getTargetTile() const { return m_targetTile; }
    
    // 设置HP
    void SetHP(int32_t nHP) { m_unitAttr.hp = nHP; }
    
    // 附加技能
    void clearAddonSkills();
    void checkAddonSkills();

protected:
    // 成员变量 (从反编译代码的构造和析构函数还原)
    bool m_bDie;                        // 是否死亡
    CObjAttrs m_baseAttr;               // 基础属性
    CObjAttrs m_buffAttrRatio;          // Buff属性比率
    CObjAttrs m_buffAttrValue;          // Buff属性值
    CObjState m_buffStates;             // Buff状态
    Buff* m_buffs[40];                  // Buff列表 (最多40个)
    Position m_currentPos;              // 当前像素位置
    Position m_currentTile;             // 当前格子位置
    int8_t m_direction;                 // 方向
    std::list<UnitAddonSkill> m_lAddonSkill;   // 附加技能
    std::list<int32_t> m_lSpeedBuffRatio;      // 速度Buff比率
    int64_t m_lastHPTick;               // 上次HP检查tick
    int64_t m_nSkillChantTime;          // 技能吟唱时间
    int32_t m_nSkillFlag;               // 技能标记
    bool m_needSync;                    // 是否需要同步
    Map* m_pMap;                        // 所在地图
    int64_t m_posUpdateTick;            // 位置更新tick
    Position m_targetPos;               // 目标像素位置
    Position m_targetTile;              // 目标格子位置
    int64_t m_throwedTick;              // 被抛掷tick
    int32_t m_xOffset;                  // X偏移
    int32_t m_yOffset;                  // Y偏移
    
    // 单位属性 (hp, mp, kun_li 等)
    struct UnitAttrData {
        int32_t hp;
        int32_t mp;
        int32_t kun_li;
    } m_unitAttr;
};

#endif // _UNIT_H_