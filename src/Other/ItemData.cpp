#include "Other/ItemData.h"

bool ItemData::IsEmpty() const
{
    return m_nId <= 0 || m_nClass <= 0 || m_nCount <= 0;
}
