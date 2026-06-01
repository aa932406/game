#include "Other/CObjAttrs.h"

CObjAttrs::CObjAttrs()
{
    m_vAttr.resize(ATTR_MAX, 0);
}

CObjAttrs::~CObjAttrs()
{
}

int32_t CObjAttrs::GetAttr(Index_T nIdx) const
{
    if (nIdx >= 0 && nIdx < ATTR_MAX) {
        return m_vAttr[nIdx];
    }
    return 0;
}

void CObjAttrs::SetAttr(Index_T nIdx, int32_t nValue)
{
    if (nIdx >= 0 && nIdx < ATTR_MAX) {
        m_vAttr[nIdx] = nValue;
    }
}

void CObjAttrs::CleanUp()
{
    for (int i = 0; i < ATTR_MAX; ++i) {
        m_vAttr[i] = 0;
    }
}