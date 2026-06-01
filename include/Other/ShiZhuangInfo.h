#ifndef _SHIZHUANGINFO_H_
#define _SHIZHUANGINFO_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class ShiZhuangInfo
{
public:
    ShiZhuangInfo();

    void CleanUp();
    void ParesActiveString(std::string * strActive);
    std::string GetActiveString();
    void AddTime(int32_t nId, int32_t nTime);
    bool IsForever(int32_t nId);
    bool IsActive(int32_t nId);

private:
    std::map<int32_t, int32_t> m_lActive;
    int32_t m_nExp;
    int32_t m_nLevel;
    int32_t m_nWear;
};

#endif
