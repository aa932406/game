#include "Other/Jingyandan.h"
#include <cstdlib>

Jingyandan::Jingyandan()
    : m_exp(0)
{
}

Jingyandan::~Jingyandan()
{
}

bool Jingyandan::parseEffect(int32_t id, const std::string* strEffect)
{
    m_exp = atoi(strEffect->c_str());
    return m_exp > 0;
}
