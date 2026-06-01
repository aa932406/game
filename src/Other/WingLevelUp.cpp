#include "Other/WingLevelUp.h"
#include "Common/Common.h"

WingLevelUp::WingLevelUp() : UseWingLevel(0), UpLevel(0), ItemId(0), ItemCount(0) {}
WingLevelUp::~WingLevelUp() {}
bool WingLevelUp::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector sv;
    Common::splistString(strEffect->c_str(), &sv, "\t");
    if (sv.size() < 4)
        return false;
    UseWingLevel = atoi(sv[0].c_str());
    UpLevel = atoi(sv[1].c_str());
    ItemId = atoi(sv[2].c_str());
    ItemCount = atoi(sv[3].c_str());
    return UseWingLevel > 0;
}
