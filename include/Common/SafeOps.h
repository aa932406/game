#ifndef _SAFEOPS_H_
#define _SAFEOPS_H_

#include <cstdint>
#include <cassert>

/**
 * @brief 安全操作工具集：空指针检查、除零保护、边界检查等
 */

// ============================================================
// 空指针检查
// ============================================================

#define SAFE_DEREF(ptr, fail_ret)  do { if (!(ptr)) return (fail_ret); } while(0)
#define SAFE_DEREF_VOID(ptr)       do { if (!(ptr)) return; } while(0)

template<typename T>
inline bool IsNullPtr(const T* ptr) { return ptr == nullptr; }

template<typename T>
inline bool IsValidPtr(const T* ptr) { return ptr != nullptr; }

// ============================================================
// 除零保护
// ============================================================

template<typename T>
inline T SafeDivide(T numerator, T denominator, T default_val = T(0))
{
    return (denominator != T(0)) ? (numerator / denominator) : default_val;
}

template<typename T>
inline T SafeModulo(T numerator, T denominator, T default_val = T(0))
{
    return (denominator != T(0)) ? (numerator % denominator) : default_val;
}

// ============================================================
// 容器安全访问
// ============================================================

template<typename Container>
inline bool IndexInBounds(const Container& c, size_t index)
{
    return index < c.size();
}

template<typename Container>
inline bool IsContainerEmpty(const Container& c)
{
    return c.empty();
}

template<typename MapType>
inline bool MapContains(const MapType& m, const typename MapType::key_type& key)
{
    return m.find(key) != m.end();
}

// ============================================================
// 类型安全转换
// ============================================================

template<typename To, typename From>
inline To SafeNarrowCast(From val)
{
    To result = static_cast<To>(val);
    assert(static_cast<From>(result) == val && "narrow cast overflow");
    return result;
}

// ============================================================
// 数值范围限制
// ============================================================

template<typename T>
inline T ClampValue(T val, T min_val, T max_val)
{
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

#endif // _SAFEOPS_H_
