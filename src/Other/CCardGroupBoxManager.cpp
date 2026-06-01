#include "Other/CCardGroupBoxManager.h"
#include "Other/CCardGroupBox.h"

int32_t CCardGroupBoxManager::GetCard(int32_t nId)
{
    for (auto iter = m_lCardGroupBox.begin(); iter != m_lCardGroupBox.end(); ++iter)
    {
        CCardGroupBox* pBox = &(*iter);
        if (pBox->m_nId == nId)
        {
            return pBox->GetCard();
        }
    }
    return 0;
}
