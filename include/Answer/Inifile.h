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
};

} // namespace Answer

#endif // _INIFILE_H_
