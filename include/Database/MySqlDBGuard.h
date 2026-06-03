#ifndef _MYSQLDBGUARD_H_
#define _MYSQLDBGUARD_H_

#include <cstdint>
#include <string>

namespace Answer {

class DBPool;
class MySqlQuery;

class MySqlDBGuard
{
public:
    MySqlDBGuard() = default;
    MySqlDBGuard(DBPool* pool) : m_pool(pool) {}
    virtual ~MySqlDBGuard() = default;

    void excute(const std::string& sql);
    void excute(const char* sql);
    static void excute(MySqlDBGuard* guard, const std::string& sql) { if (guard) guard->excute(sql); }
    static void excute(MySqlDBGuard* guard, const char* sql) { if (guard) guard->excute(sql); }
    MySqlQuery* query(const std::string& sql);
    MySqlQuery* query(const char* sql);
    static MySqlQuery* query(MySqlDBGuard* guard, const std::string& sql) { return guard ? guard->query(sql) : nullptr; }
    static MySqlQuery* query(MySqlDBGuard* guard, const char* sql) { return guard ? guard->query(sql) : nullptr; }

    int64_t getInsertId();
    int32_t getAffectedRows();

private:
    DBPool* m_pool = nullptr;
};

} // namespace Answer

#endif // _MYSQLDBGUARD_H_
