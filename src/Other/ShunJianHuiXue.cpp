#include "Other/ShunJianHuiXue.h"
#include <cstdlib>

ShunJianHuiXue::ShunJianHuiXue()
    : m_addon(0)
{
}

ShunJianHuiXue::~ShunJianHuiXue()
{
}

bool ShunJianHuiXue::parseEffect(int32_t id, const std::string* strEffect)
{
    m_addon = atoi(strEffect->c_str());
    return m_addon > 0;
}
