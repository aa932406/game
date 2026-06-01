#ifndef _COBJATTRS_H_
#define _COBJATTRS_H_

#include <cstdint>
#include <vector>

class CObjAttrs
{
public:
    // 属性索引枚举 (从反编译代码还原)
    enum Index_T
    {
        ATTR_HP = 0,            // 血量上限
        ATTR_MP = 1,            // 法力上限
        ATTR_KUN_LI = 2,        // 坤力
        ATTR_PHYSICAL_ATTACK = 3,   // 物理攻击
        ATTR_PHYSICAL_DEFENSE = 4,  // 物理防御
        ATTR_MAGIC_ATTACK = 5,      // 法术攻击
        ATTR_MAGIC_DEFENSE = 6,     // 法术防御
        ATTR_MOV_SPD = 7,           // 移动速度
        ATTR_ATTACK_SPD = 8,        // 攻击速度
        ATTR_XUAN_YUN_DEF = 9,      // 眩晕防御
        // ... 更多属性
        ATTR_MAX = 50
    };

    CObjAttrs();
    ~CObjAttrs();

    // 获取/设置属性
    int32_t GetAttr(Index_T nIdx) const;
    void SetAttr(Index_T nIdx, int32_t nValue);
    
    // 清理
    void CleanUp();

private:
    std::vector<int32_t> m_vAttr;   // 属性数组
};

#endif // _COBJATTRS_H_