#include "Other/CObjState.h"

CObjState::CObjState()
{
    m_vState.resize(OBS_MAX, false);
}

CObjState::~CObjState()
{
}

bool CObjState::GetState(Index_T nIdx) const
{
    if (nIdx >= 0 && nIdx < OBS_MAX) {
        return m_vState[nIdx];
    }
    return false;
}

void CObjState::AddState(Index_T nIdx)
{
    if (nIdx >= 0 && nIdx < OBS_MAX) {
        m_vState[nIdx] = true;
    }
}

void CObjState::RemoveState(Index_T nIdx)
{
    if (nIdx >= 0 && nIdx < OBS_MAX) {
        m_vState[nIdx] = false;
    }
}

void CObjState::CleanUp()
{
    for (int i = 0; i < OBS_MAX; ++i) {
        m_vState[i] = false;
    }
}