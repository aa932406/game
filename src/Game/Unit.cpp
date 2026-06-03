#include "common.h"
#include "Game/Unit.h"
#include "Game/Map.h"
#include "Game/CTimer.h"
#include "Game/GameService.h"
#include "Other/StaticObj.h"
#include "Other/EntityType.h"
#include "Other/CObjAttrs.h"
#include "Other/CObjState.h"
#include "Other/CBuff.h"
#include "Other/InstanceMoveReason.h"
#include "Other/ItemChangeReason.h"
#include "Utility/UnitHandle.h"

#include <ctime>
#include <cmath>
#include <limits>
#include <algorithm>
#include <list>
#include <string>

// ============================================================
// 构造函数和析构函数
// ============================================================

Unit::Unit(EntityType entitytype)
    : StaticObj(entitytype)
    , m_bDie(false)
    , m_direction(0)
    , m_lastHPTick(0)
    , m_nSkillChantTime(0)
    , m_nSkillFlag(0)
    , m_needSync(false)
    , m_pMap(nullptr)
    , m_posUpdateTick(0)
    , m_throwedTick(0)
    , m_xOffset(0)
    , m_yOffset(0)
    , m_Camp(0)
{
    m_targetTile = Position(0, 0);
    m_targetPos = Position(0, 0);
    m_currentTile = Position(0, 0);
    m_currentPos = Position(0, 0);

    m_unitAttr.hp = 0;
    m_unitAttr.mp = 0;
    m_unitAttr.kun_li = 0;

    // 初始化Buff数组
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
        m_buffs[i] = nullptr;

    // 清除Buff Buffer
    memset(m_buffBuffer, 0, sizeof(m_buffBuffer));
}

Unit::~Unit()
{
    // 清理Buff
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            delete m_buffs[i];
            m_buffs[i] = nullptr;
        }
    }
    m_lSpeedBuffRatio.clear();
    m_lAddonSkill.clear();
}

// ============================================================
// 基础方法
// ============================================================

UnitHandle Unit::getHandle() const
{
    return UnitHandle(getId(), getType());
}

void Unit::reset()
{
    StaticObj::reset();

    m_baseAttr.CleanUp();
    m_buffAttrRatio.CleanUp();
    m_buffAttrValue.CleanUp();

    m_bDie = false;
    m_needSync = false;
    m_pMap = nullptr;
    m_direction = static_cast<int8_t>(Direction::DOWN);
    m_nSkillFlag = 0;
    m_nSkillChantTime = 0;
    m_posUpdateTick = 0;
    m_throwedTick = 0;
    m_lastHPTick = 0;
    m_xOffset = 0;
    m_yOffset = 0;
    m_Camp = 0;

    m_targetTile = Position(0, 0);
    m_targetPos = Position(0, 0);
    m_currentTile = Position(0, 0);
    m_currentPos = Position(0, 0);

    m_unitAttr.hp = 0;
    m_unitAttr.mp = 0;
    m_unitAttr.kun_li = 0;

    m_lAddonSkill.clear();
    m_lSpeedBuffRatio.clear();

    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            delete m_buffs[i];
            m_buffs[i] = nullptr;
        }
    }
}

void Unit::update()
{
    // 检查HP回复
    checkHPRef();

    // 检查Buff列表
    checkBuffList();

    // 检查附加技能
    checkAddonSkills();

    // 同步基础数据
    syncBasicData();
}

void Unit::checkHPRef()
{
    if (!isAlive())
        return;

    int64_t curTick = getTick();
    if (curTick - m_lastHPTick > 10000)
    {
        m_lastHPTick = curTick;
        // HP回复逻辑
        int64_t maxHP = GetMaxHP();
        int32_t addHP = static_cast<int32_t>(maxHP * 2 / 100); // 每10秒回复2%
        if (addHP > 0)
            AddHP(addHP);
    }
}

void Unit::leaveTile()
{
    m_currentTile = Position(0, 0);
    m_targetTile = m_currentTile;
    m_currentPos = Position(0, 0);
    m_targetPos = m_currentPos;
}

void Unit::setAtTile(Map* pMap, int32_t tx, int32_t ty)
{
    m_pMap = pMap;
    m_currentTile = Position(tx, ty);
    m_targetTile = m_currentTile;

    if (pMap)
        m_currentPos = Map::tileToPixel(tx, ty);
    else
        m_currentPos = Position(0, 0);

    m_targetPos = m_currentPos;
}

void Unit::setTargetTile(int32_t tx, int32_t ty)
{
    if (!m_pMap)
        return;

    if (m_pMap->isWalkablePosition(tx, ty))
    {
        m_targetTile.x = tx;
        m_targetTile.y = ty;
        m_targetPos = Map::tileToPixel(tx, ty);
    }
}

bool Unit::resetDirection(int32_t tx, int32_t ty)
{
    int32_t dx = tx - m_currentTile.x;
    int32_t dy = ty - m_currentTile.y;

    if (dx == 0 && dy == 0)
        return false;

    if (std::abs(dx) >= std::abs(dy))
    {
        m_direction = static_cast<int8_t>(dx > 0 ? Direction::RIGHT : Direction::LEFT);
    }
    else
    {
        m_direction = static_cast<int8_t>(dy > 0 ? Direction::DOWN : Direction::UP);
    }

    return true;
}

void Unit::instantMove(int32_t tx, int32_t ty, InstanceMoveReason reason, int8_t dir)
{
    if (!m_pMap)
        return;

    m_currentTile = Position(tx, ty);
    m_targetTile = m_currentTile;
    m_currentPos = Map::tileToPixel(tx, ty);
    m_targetPos = m_currentPos;

    if (dir >= 0)
        m_direction = dir;

    broadcastMove();
}

void Unit::setStand()
{
    if (m_currentPos.x != m_targetPos.x || m_currentPos.y != m_targetPos.y)
    {
        m_targetTile = m_currentTile;
        m_targetPos = m_currentPos;
        m_posUpdateTick = getTick();

        if (getType() != EntityType::ET_MONSTER)
            broadcastMove();
    }
}

bool Unit::isInRectangle(Position pst1, Position pst2) const
{
    return m_currentTile.x >= pst1.x
        && m_currentTile.x <= pst2.x
        && m_currentTile.y >= pst1.y
        && m_currentTile.y <= pst2.y;
}

bool Unit::updatePosition()
{
    if (!m_pMap || !isMoving())
        return false;

    int64_t curTick = getTick();
    if (curTick - m_posUpdateTick <= 0)
        return false;

    double speed = getDirectionSpeed();
    if (speed <= 0.0)
        return false;

    int64_t timeDelta = curTick - m_posUpdateTick;
    double distance = speed * timeDelta / 1000.0;

    double dx = static_cast<double>(m_targetPos.x - m_currentPos.x);
    double dy = static_cast<double>(m_targetPos.y - m_currentPos.y);
    double totalDist = std::sqrt(dx * dx + dy * dy);

    if (totalDist <= 0.0)
    {
        m_currentPos = m_targetPos;
        m_currentTile = m_targetTile;
        return false;
    }

    if (distance >= totalDist)
    {
        m_currentPos = m_targetPos;
        m_currentTile = m_targetTile;
    }
    else
    {
        double ratio = distance / totalDist;
        m_currentPos.x = m_currentPos.x + static_cast<int32_t>(dx * ratio);
        m_currentPos.y = m_currentPos.y + static_cast<int32_t>(dy * ratio);
    }

    m_posUpdateTick = curTick;
    return !isMoving();
}

bool Unit::isMoving() const
{
    return m_currentPos.x != m_targetPos.x || m_currentPos.y != m_targetPos.y;
}

int64_t Unit::getTick() const
{
    if (m_pMap)
        return Map::getTick(m_pMap);

    return CTimer::GetTick();
}

int32_t Unit::getNow() const
{
    if (m_pMap)
        return Map::getNow(m_pMap);

    return CTimer::GetNow();
}

void Unit::getLocalNow(struct tm* retstr) const
{
    if (m_pMap)
        Map::getLocalNow(retstr, m_pMap);
    else
        CTimer::GetLocalNow(retstr);
}

double Unit::pixelDistance(const Unit* unit) const
{
    if (!unit || StaticObj::getMap(unit) != m_pMap)
        return static_cast<double>(std::numeric_limits<int32_t>::max());

    return m_currentPos.distance(unit->m_currentPos);
}

int32_t Unit::tileDistance(const Unit* unit) const
{
    if (!unit)
        return std::numeric_limits<int32_t>::max();

    return m_currentTile.tileDistance(unit->m_currentTile);
}

bool Unit::isAlive() const
{
    return !m_bDie;
}

// ============================================================
// 属性相关
// ============================================================

int32_t Unit::GetBaseAttr(CObjAttrs::Index_T nIdx) const
{
    int32_t val = m_baseAttr.GetAttr(nIdx);
    return val > 0 ? val : 0;
}

int32_t Unit::GetAttrValue(CObjAttrs::Index_T nIdx) const
{
    int32_t base = GetBaseAttr(nIdx);
    int32_t buffValue = GetBuffAttrValue(nIdx);
    int32_t buffRatio = GetBuffAttrRatio(nIdx);

    return static_cast<int32_t>((base + buffValue) * (1.0f + buffRatio / 100.0f));
}

int32_t Unit::AddAttrValue(CObjAttrs::Index_T nIdx, int32_t nAddVal)
{
    if (nAddVal == 0)
        return m_baseAttr.GetAttr(nIdx);

    int32_t nRealValue = nAddVal + m_baseAttr.GetAttr(nIdx);
    m_baseAttr.SetAttr(nIdx, nRealValue);
    m_needSync = true;
    return nRealValue;
}

void Unit::SetAttrValue(CObjAttrs::Index_T nIdx, int32_t nVal)
{
    m_baseAttr.SetAttr(nIdx, nVal);
    m_needSync = true;
}

int32_t Unit::GetBuffAttrRatio(CObjAttrs::Index_T nIdx) const
{
    int32_t nRatio = m_buffAttrRatio.GetAttr(nIdx);

    if (nIdx == CObjAttrs::ATTR_MOV_SPD)
        nRatio += GetSpeedBuffRatio();

    return nRatio;
}

int32_t Unit::AddBuffAttrRatio(CObjAttrs::Index_T nIdx, int32_t nRate)
{
    if (nRate == 0)
        return m_buffAttrRatio.GetAttr(nIdx);

    int32_t attr = m_buffAttrRatio.GetAttr(nIdx);
    m_buffAttrRatio.SetAttr(nIdx, nRate + attr);
    m_needSync = true;
    return nRate + attr;
}

int32_t Unit::GetBuffAttrValue(CObjAttrs::Index_T nIdx) const
{
    return m_buffAttrValue.GetAttr(nIdx);
}

int32_t Unit::AddBuffAttrValue(CObjAttrs::Index_T nIdx, int32_t nAddVal)
{
    if (nAddVal == 0)
        return m_buffAttrValue.GetAttr(nIdx);

    int32_t attr = m_buffAttrValue.GetAttr(nIdx);
    m_buffAttrValue.SetAttr(nIdx, nAddVal + attr);
    m_needSync = true;
    return nAddVal + attr;
}

// ============================================================
// Buff状态管理
// ============================================================

bool Unit::HasBuffState(CObjState::Index_T nIdx) const
{
    return m_buffStates.GetState(nIdx);
}

void Unit::AddBuffState(CObjState::Index_T nIdx)
{
    m_buffStates.AddState(nIdx);
    m_needSync = true;
}

void Unit::RemoveBuffState(CObjState::Index_T nIdx)
{
    m_buffStates.RemoveState(nIdx);
    m_needSync = true;
}

// ============================================================
// Buff数组管理
// ============================================================

bool Unit::HasBuffById(int32_t buffId) const
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getId() == buffId)
            return true;
    }
    return false;
}

int32_t Unit::getBuffCount() const
{
    int32_t count = 0;
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
            ++count;
    }
    return count;
}

int32_t Unit::addBuff(Buff* buff)
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (!m_buffs[i])
        {
            m_buffs[i] = buff;
            broadcastAddBuff(static_cast<int8_t>(i), buff);
            m_needSync = true;
            return i;
        }
    }
    return -1;
}

void Unit::removeBuff(int32_t buffId)
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getId() == buffId)
        {
            delete m_buffs[i];
            m_buffs[i] = nullptr;
            broadcastBuffEnd(static_cast<int8_t>(i), 0);
            m_needSync = true;
            return;
        }
    }
}

void Unit::removeBuffBySpecail(int32_t special)
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getSpecial() == special)
        {
            delete m_buffs[i];
            m_buffs[i] = nullptr;
            broadcastBuffEnd(static_cast<int8_t>(i), 0);
            m_needSync = true;
        }
    }
}

void Unit::clearBuff()
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            int32_t special = m_buffs[i]->getSpecial();
            if (special != 1) // 不删移除不可清除的Buff
            {
                delete m_buffs[i];
                m_buffs[i] = nullptr;
                broadcastBuffEnd(static_cast<int8_t>(i), 0);
            }
        }
    }
    m_needSync = true;
}

void Unit::clearContrlBuff()
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            int32_t special = m_buffs[i]->getSpecial();
            if (special == 2) // 控制类Buff
            {
                delete m_buffs[i];
                m_buffs[i] = nullptr;
                broadcastBuffEnd(static_cast<int8_t>(i), 0);
            }
        }
    }
    m_needSync = true;
}

void Unit::clearDebuff(int32_t nCount)
{
    int32_t cleared = 0;
    for (int32_t i = 0; i < MAX_BUFF_COUNT && cleared < nCount; ++i)
    {
        if (m_buffs[i])
        {
            int32_t special = m_buffs[i]->getSpecial();
            if (special == 2) // Debuff类
            {
                delete m_buffs[i];
                m_buffs[i] = nullptr;
                broadcastBuffEnd(static_cast<int8_t>(i), 0);
                ++cleared;
            }
        }
    }
    if (cleared > 0)
        m_needSync = true;
}

Buff* Unit::findBuffBySpecail(int32_t special)
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getSpecial() == special)
            return m_buffs[i];
    }
    return nullptr;
}

bool Unit::checkBuffBySpecail(int32_t special) const
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getSpecial() == special)
            return true;
    }
    return false;
}

void Unit::getItemBuffs(MemChrBuffVector* retstr) const
{
    retstr->clear();
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            MemChrBuff chrbuff;
            chrbuff.buffId = m_buffs[i]->getId();
            chrbuff.index = static_cast<int8_t>(i);
            retstr->push_back(chrbuff);
        }
    }
}

void Unit::checkBuffList()
{
    if (!isAlive())
    {
        deathClearBuff();
        return;
    }

    int64_t curTick = getTick();
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getEndTick() > 0 && curTick >= m_buffs[i]->getEndTick())
        {
            delete m_buffs[i];
            m_buffs[i] = nullptr;
            broadcastBuffEnd(static_cast<int8_t>(i), 1);
            m_needSync = true;
        }
    }
}

void Unit::deathClearBuff()
{
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            int32_t special = m_buffs[i]->getSpecial();
            if (special != 1) // 死亡时不清除特殊Buff
            {
                delete m_buffs[i];
                m_buffs[i] = nullptr;
                broadcastBuffEnd(static_cast<int8_t>(i), 2);
            }
        }
    }
    m_needSync = true;
}

// ============================================================
// HP/MP相关
// ============================================================

int64_t Unit::GetMaxHP() const
{
    return GetAttrValue(CObjAttrs::ATTR_MAX_HP);
}

int32_t Unit::GetViewHP() const
{
    return m_unitAttr.hp;
}

int32_t Unit::GetViewMaxHP() const
{
    return static_cast<int32_t>(GetMaxHP());
}

void Unit::AddHP(int32_t nAddValue)
{
    int64_t maxHP = GetMaxHP();
    int32_t newHP = m_unitAttr.hp + nAddValue;

    if (newHP > static_cast<int32_t>(maxHP))
        newHP = static_cast<int32_t>(maxHP);
    if (newHP < 0)
        newHP = 0;

    m_unitAttr.hp = newHP;
    m_needSync = true;
}

void Unit::SetHP(int32_t nHP)
{
    int64_t maxHP = GetMaxHP();
    if (nHP > static_cast<int32_t>(maxHP))
        nHP = static_cast<int32_t>(maxHP);
    if (nHP < 0)
        nHP = 0;

    m_unitAttr.hp = nHP;
    m_needSync = true;
}

void Unit::AddKunLi(int32_t nAddValue)
{
    int64_t maxKunLi = GetMaxKunLi();
    int32_t newKunLi = m_unitAttr.kun_li + nAddValue;

    if (newKunLi > static_cast<int32_t>(maxKunLi))
        newKunLi = static_cast<int32_t>(maxKunLi);
    if (newKunLi < 0)
        newKunLi = 0;

    m_unitAttr.kun_li = newKunLi;
    m_needSync = true;
}

int64_t Unit::GetMaxKunLi() const
{
    return GetAttrValue(CObjAttrs::ATTR_MAX_KUN_LI);
}

void Unit::AddMP(int32_t nAddValue)
{
    int32_t newMP = m_unitAttr.mp + nAddValue;
    if (newMP < 0)
        newMP = 0;

    m_unitAttr.mp = newMP;
    m_needSync = true;
}

void Unit::FillHP(int32_t nPercent)
{
    int64_t maxHP = GetMaxHP();
    int32_t addHP = static_cast<int32_t>(maxHP * nPercent / 100);
    AddHP(addHP);
}

void Unit::FillMP(int32_t nPercent)
{
    int32_t maxMP = static_cast<int32_t>(GetAttrValue(CObjAttrs::ATTR_MAX_MP));
    int32_t addMP = maxMP * nPercent / 100;
    AddMP(addMP);
}

// ============================================================
// 技能标记
// ============================================================

int32_t Unit::GetSkillFlag(bool bIncrease)
{
    if (bIncrease && getType() != EntityType::ET_PLAYER)
        ++m_nSkillFlag;

    return m_nSkillFlag;
}

// ============================================================
// 属性调整
// ============================================================

void Unit::resetUnitAttr()
{
    m_unitAttr.hp = 0;
    m_unitAttr.mp = 0;
    m_unitAttr.kun_li = 0;
}

void Unit::adjustUnitAttr()
{
    int64_t maxHP = GetMaxHP();
    if (m_unitAttr.hp > static_cast<int32_t>(maxHP))
        m_unitAttr.hp = static_cast<int32_t>(maxHP);
    if (m_unitAttr.hp < 0)
        m_unitAttr.hp = 0;

    int64_t maxMP = GetAttrValue(CObjAttrs::ATTR_MAX_MP);
    if (m_unitAttr.mp > static_cast<int32_t>(maxMP))
        m_unitAttr.mp = static_cast<int32_t>(maxMP);
    if (m_unitAttr.mp < 0)
        m_unitAttr.mp = 0;

    int64_t maxKunLi = GetMaxKunLi();
    if (m_unitAttr.kun_li > static_cast<int32_t>(maxKunLi))
        m_unitAttr.kun_li = static_cast<int32_t>(maxKunLi);
    if (m_unitAttr.kun_li < 0)
        m_unitAttr.kun_li = 0;
}

void Unit::ResetAttrs(Unit* unit)
{
    if (unit)
        unit->resetUnitAttr();
}

// ============================================================
// 速度Buff比率管理
// ============================================================

int32_t Unit::AddSpeedBuffRatio(int32_t nRatio)
{
    m_lSpeedBuffRatio.push_back(nRatio);
    m_lSpeedBuffRatio.sort(std::greater<int32_t>());
    m_needSync = true;
    return nRatio;
}

int32_t Unit::RemoveSpeedBuffRatio(int32_t nRatio)
{
    for (auto iter = m_lSpeedBuffRatio.begin(); iter != m_lSpeedBuffRatio.end(); ++iter)
    {
        if (*iter == nRatio)
        {
            m_lSpeedBuffRatio.erase(iter);
            m_needSync = true;
            return nRatio;
        }
    }
    return 0;
}

int32_t Unit::GetSpeedBuffRatio() const
{
    if (m_lSpeedBuffRatio.empty())
        return 0;

    int32_t top = m_lSpeedBuffRatio.front();
    if (top <= 0)
        return 0;

    return top;
}

// ============================================================
// 速度计算
// ============================================================

double Unit::getDirectionSpeed() const
{
    int32_t moveSpeed = GetAttrValue(CObjAttrs::ATTR_MOV_SPD);
    if (moveSpeed <= 0)
        return 0.0;

    double speed = static_cast<double>(moveSpeed) * 0.05;
    return speed;
}

// ============================================================
// 伤害处理
// ============================================================

int32_t Unit::struckDamage(int32_t damage, Unit* pAttacker, int32_t skillId)
{
    if (damage <= 0 || !isAlive())
        return 0;

    // 护盾减伤
    damage = shieldDamage(damage);

    if (damage <= 0)
        return 0;

    // 应用伤害
    AddHP(-damage);

    // 检查死亡
    if (m_unitAttr.hp <= 0)
    {
        m_bDie = true;
        m_unitAttr.hp = 0;
    }

    return damage;
}

int32_t Unit::shieldDamage(int32_t damage)
{
    // 检查护盾Buff
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i] && m_buffs[i]->getSpecial() == 5) // 护盾类Buff
        {
            int32_t shieldValue = m_buffs[i]->getParam(0);
            if (shieldValue > 0)
            {
                if (damage >= shieldValue)
                {
                    damage -= shieldValue;
                    m_buffs[i]->setParam(0, 0);
                }
                else
                {
                    m_buffs[i]->setParam(0, shieldValue - damage);
                    damage = 0;
                }
            }
        }
    }

    return damage;
}

void Unit::onThrowed()
{
    m_throwedTick = getTick() + 800;
    broadcastThrowed();
}

// ============================================================
// 同步和广播
// ============================================================

void Unit::syncBasicData()
{
    if (m_needSync)
    {
        broadcastBasicData();
        m_needSync = false;
    }
}

void Unit::broadcastMove()
{
    if (!m_pMap)
        return;

    // 发送移动包
    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x29);
    if (!packet)
        return;

    EntityId_t id = getId();
    int8_t type = static_cast<int8_t>(getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt16(packet, m_currentTile.x);
    Answer::NetPacket::writeInt16(packet, m_currentTile.y);
    Answer::NetPacket::writeInt16(packet, m_targetTile.x);
    Answer::NetPacket::writeInt16(packet, m_targetTile.y);
    Answer::NetPacket::writeInt8(packet, m_direction);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Unit::broadcastMoveMonster(Position Pos)
{
    if (!m_pMap)
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x2A);
    if (!packet)
        return;

    EntityId_t id = getId();
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt16(packet, Pos.x);
    Answer::NetPacket::writeInt16(packet, Pos.y);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Unit::broadcastInstantMove(Unit* pUnit)
{
    if (!pUnit || !pUnit->m_pMap)
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x2B);
    if (!packet)
        return;

    EntityId_t id = pUnit->getId();
    int8_t type = static_cast<int8_t>(pUnit->getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt16(packet, pUnit->m_currentTile.x);
    Answer::NetPacket::writeInt16(packet, pUnit->m_currentTile.y);
    Answer::NetPacket::writeInt8(packet, pUnit->m_direction);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(pUnit->m_pMap, packet, pUnit);
}

void Unit::broadcastKickOutedMove()
{
    // 被踢出的移动广播
    broadcastMove();
}

void Unit::broadcastThrowed()
{
    if (!m_pMap)
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x2D);
    if (!packet)
        return;

    EntityId_t id = getId();
    int8_t type = static_cast<int8_t>(getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt16(packet, m_targetTile.x);
    Answer::NetPacket::writeInt16(packet, m_targetTile.y);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Unit::broadcastLeave()
{
    if (m_pMap)
        Map::broadcastLeaveMap(m_pMap, this);
}

void Unit::broadcastBasicData()
{
    // 基础数据同步 - 由子类实现
}

void Unit::broadcastSkillChant(int32_t nSkillId, int32_t nChantTime)
{
    if (!m_pMap)
        return;

    m_nSkillChantTime = nChantTime;
    m_nSkillFlag = nSkillId;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x2E);
    if (!packet)
        return;

    EntityId_t id = getId();
    int8_t type = static_cast<int8_t>(getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt32(packet, nSkillId);
    Answer::NetPacket::writeInt32(packet, nChantTime);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Unit::broadcastBreakSkillChant()
{
    if (!m_pMap || m_nSkillChantTime <= 0)
        return;

    m_nSkillChantTime = 0;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x2F);
    if (!packet)
        return;

    EntityId_t id = getId();
    int8_t type = static_cast<int8_t>(getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Unit::packageBuffList(Answer::NetPacket* packet) const
{
    int32_t count = 0;
    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
            ++count;
    }

    Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(count));

    for (int32_t i = 0; i < MAX_BUFF_COUNT; ++i)
    {
        if (m_buffs[i])
        {
            Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(i));
            Answer::NetPacket::writeInt32(packet, m_buffs[i]->getId());
        }
    }
}

void Unit::broadcastAddBuff(int8_t nIndex, Buff* pBuff)
{
    if (!m_pMap || !pBuff)
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x30);
    if (!packet)
        return;

    EntityId_t id = getId();
    int8_t type = static_cast<int8_t>(getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt8(packet, nIndex);
    Answer::NetPacket::writeInt32(packet, pBuff->getId());

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void Unit::broadcastBuffEnd(int8_t nIndex, int32_t nReason)
{
    Int8List buffs;
    buffs.push_back(nIndex);
    broadcastBuffEnd(&buffs, nReason);
}

void Unit::broadcastBuffEnd(const Int8List* buffs, int32_t nReason)
{
    if (!m_pMap || !buffs || buffs->empty())
        return;

    GameService *pGameService = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, -1, Answer::PackType::PACK_DISPATCH, 0x31);
    if (!packet)
        return;

    EntityId_t id = getId();
    int8_t type = static_cast<int8_t>(getType());

    Answer::NetPacket::writeInt8(packet, type);
    Answer::NetPacket::writeInt64(packet, id);
    Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(buffs->size()));
    Answer::NetPacket::writeInt32(packet, nReason);

    for (auto iter = buffs->begin(); iter != buffs->end(); ++iter)
    {
        Answer::NetPacket::writeInt8(packet, *iter);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

// ============================================================
// 附加技能管理
// ============================================================

void Unit::clearAddonSkills()
{
    if (!m_lAddonSkill.empty())
        m_lAddonSkill.clear();
}

void Unit::checkAddonSkills()
{
    // 附加技能检查逻辑 - 由子类实现
}

// ============================================================
// 方向访问器
// ============================================================

Direction Unit::getDirection() const
{
    return static_cast<Direction>(m_direction);
}

void Unit::setDirection(Direction dir)
{
    m_direction = static_cast<int8_t>(dir);
}

Direction Unit::getDirectionTo(int32_t tx, int32_t ty) const
{
    int32_t dx = tx - m_currentTile.x;
    int32_t dy = ty - m_currentTile.y;

    if (dx == 0 && dy == 0)
        return getDirection();

    double angle = std::atan2(static_cast<double>(dy), static_cast<double>(dx));
    double pi = 3.141592653589793;

    if (angle < -pi * 3.0 / 4.0 || angle >= pi * 3.0 / 4.0)
        return Direction::LEFT;
    if (angle < -pi / 4.0)
        return Direction::DOWN;
    if (angle < pi / 4.0)
        return Direction::RIGHT;
    return Direction::UP;
}

Direction Unit::getDirectionTo(const Unit* target) const
{
    if (!target)
        return getDirection();
    return getDirectionTo(target->m_currentTile.x, target->m_currentTile.y);
}

Position Unit::getTargetTile() const
{
    return m_targetTile;
}

Position Unit::getCurrentTile() const
{
    return m_currentTile;
}

Map* Unit::getMap() const
{
    return m_pMap;
}
