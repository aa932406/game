#ifndef _MD5_H_
#define _MD5_H_

#include <string>

namespace Answer {

class MD5 {
public:
    MD5() {}
    ~MD5() {}
    std::string md5sum(const std::string& input) { (void)input; return ""; }
};

} // namespace Answer

#endif // _MD5_H_
