#ifndef _MYSQLQUERY_H_
#define _MYSQLQUERY_H_

#include <cstdint>
#include <string>

namespace Answer {

class MySqlQuery
{
public:
    MySqlQuery() = default;
    MySqlQuery(MySqlQuery* other);
    virtual ~MySqlQuery() = default;

    bool eof() const;
    void nextRow();

    int32_t getIntValue(const std::string& field, int32_t defaultValue) const;
    int64_t getInt64Value(const std::string& field, int64_t defaultValue) const;
    std::string getStringValue(const std::string& field, const std::string& defaultValue) const;
    float getFloatValue(const std::string& field, float defaultValue) const;

    static int32_t getInt(MySqlQuery* q, const std::string& field, int32_t defaultValue) { return q ? q->getIntValue(field, defaultValue) : defaultValue; }
    static int32_t getInt(MySqlQuery* q, int32_t defaultValue) { (void)q; return defaultValue; }
    static int32_t getInt(MySqlQuery* q, int32_t* defaultValue) { (void)q; (void)defaultValue; return 0; }
};

} // namespace Answer

#endif // _MYSQLQUERY_H_
