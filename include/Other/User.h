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

    int64_t getUid() const { return m_uid; }
    int32_t getSid() const { return m_sid; }
    void setCid(int32_t cid) { m_cid = cid; }
    void setPlayer(Player* player) { m_player = player; }

    // 静态包装
    static Player* getPlayer(User* user) { return user ? user->m_player : nullptr; }
    static void GetTencentInfo(TencentInfo* retstr, User* user) { if (user && retstr) user->GetTencentInfo(*retstr); }
    static void setPlayer(User* user, Player* player) { if (user) user->setPlayer(player); }
    static int64_t getUid(User* user) { return user ? user->m_uid : 0; }
    static int32_t getSid(User* user) { return user ? user->m_sid : 0; }
    static void setCid(User* user, int32_t cid) { if (user) user->setCid(cid); }

private:
    int64_t m_uid;
    int32_t m_sid;
    int32_t m_cid;
    Player* m_player;
    TencentInfo m_tencentInfo;
};

#endif // _USER_H_
