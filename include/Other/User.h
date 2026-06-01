#ifndef _USER_H_
#define _USER_H_

#include <cstdint>

#include "Other/TencentInfo.h"

class Player;

class User
{
public:
    void init(int64_t uid, int32_t sid);
    void reset();
    void SetTencentInfo(const TencentInfo* info);
    void GetTencentInfo(TencentInfo& retstr);

private:
    int64_t m_uid;
    int32_t m_sid;
    int32_t m_cid;
    Player* m_player;
    TencentInfo m_tencentInfo;
};

#endif // _USER_H_
