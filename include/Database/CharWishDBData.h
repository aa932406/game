#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace Answer {
    class MySqlDBGuard;
    class MySqlQuery;
    class NetPacket;
}

typedef int64_t CharId_t;
typedef std::list<std::string> SqlStringList;

#ifndef CHAR_WISH_INFO_DEFINED
#define CHAR_WISH_INFO_DEFINED
struct CharWishInfo
{
    int32_t nId;
    int32_t nItemId;
    int32_t nStartTime;
    int32_t nGetReward;
    std::string strInfo;
};
#endif

class CharWishDBData {
public:
    CharWishDBData();
    ~CharWishDBData();
    void SaveToSqlString(SqlStringList *sqls, char (*szSQL)[4096], CharId_t nCid);
    bool LoadFromDB(Answer::MySqlDBGuard *db, char (*szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid);
    void PackageData(Answer::NetPacket *packet);

public:
    std::list<CharWishInfo> lstWishs;
};
