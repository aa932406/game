#ifndef _MYSQLDBGUARD_H_
#define _MYSQLDBGUARD_H_

#include <cstdint>
#include <string>

namespace Answer {

class MySqlQuery;

class MySqlDBGuard
{
public:
    MySqlDBGuard() = default;
    virtual ~MySqlDBGuard() = default;

    MySqlQuery* query(const std::string& sql);
    MySqlQuery* query(const char* sql);

    int64_t getInsertId();
    int32_t getAffectedRows();
};

} // namespace Answer

#endif // _MYSQLDBGUARD_H_
