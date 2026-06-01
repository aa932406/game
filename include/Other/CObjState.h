#ifndef _COBJSTATE_H_
#define _COBJSTATE_H_

#include <cstdint>
#include <vector>

class CObjState
{
public:
    // 状态索引枚举 (从反编译代码还原)
    enum Index_T
    {
        OBS_NONE = 0,
        OBS_STUN = 1,          // 眩晕
        OBS_SILENCE = 2,       // 沉默
        OBS_SLEEP = 3,         // 睡眠
        OBS_PLYSIS = 4,        // 麻痹
        OBS_FROZEN = 5,        // 冰冻
        OBS_WUDI = 6,          // 无敌
        OBS_INVISIBLE = 7,     // 隐身
        // ... 更多状态
        OBS_MAX = 20
    };

    CObjState();
    ~CObjState();

    // 获取/设置/移除状态
    bool GetState(Index_T nIdx) const;
    void AddState(Index_T nIdx);
    void RemoveState(Index_T nIdx);
    
    // 清理
    void CleanUp();

private:
    std::vector<bool> m_vState;  // 状态数组
};

#endif // _COBJSTATE_H_