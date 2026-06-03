#ifndef _INIFILE_H_
#define _INIFILE_H_

#include <string>

namespace Answer {

class Inifile {
public:
    Inifile() {}
    ~Inifile() {}

    void parse(const std::string* filename) { (void)filename; }
    std::string getStrValue(const std::string& key) { (void)key; return ""; }
    std::string getStrValue(const std::string* section, const std::string* key) { (void)section; (void)key; return ""; }

    static std::string GetString(Inifile* ini, const std::string& section, const std::string& key, const std::string& defaultVal) {
        (void)ini; (void)section; (void)key; return defaultVal;
    }
    static std::string GetString(Inifile* ini, const char* section, const char* key, const char* defaultVal) {
        (void)ini; (void)section; (void)key; return defaultVal ? std::string(defaultVal) : std::string();
    }

    static int32_t GetInt(Inifile* ini, const std::string& section, const std::string& key, int32_t defaultVal) {
        (void)ini; (void)section; (void)key; return defaultVal;
    }
};

} // namespace Answer

#endif // _INIFILE_H_
