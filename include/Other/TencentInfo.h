#ifndef _TENCENTINFO_H_
#define _TENCENTINFO_H_

#include <cstdint>
#include <string>

namespace Answer { class NetPacket; }

class TencentInfo
{
public:
    TencentInfo();
    TencentInfo(const TencentInfo& other);
    TencentInfo& operator=(const TencentInfo& other);

    void CleanUp();
    void UnPacketInfo(Answer::NetPacket* packet);
    int32_t GetEndTime() const;
    bool isYellowEnter() const;
    bool isBlueEnter() const;
    bool isTGPEnter() const;
    int32_t getTGPIcon() const;

private:
    std::string m_pf;
    int8_t m_bYellowVip;
    int8_t m_bYellowYearVip;
    int8_t m_bYellowHighVip;
    int8_t m_nYellowVipLevel;
    int8_t m_bBlueVip;
    int8_t m_bBlueYearVip;
    int8_t m_bBlueHighVip;
    int8_t m_nBlueVipLevel;
    int64_t m_nYearVipEndTime;
    int64_t m_nSuperVipEndTime;
    int64_t m_nExpendVipEndTime;
};

#endif // _TENCENTINFO_H_
