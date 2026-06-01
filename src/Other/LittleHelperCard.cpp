#include "Other/LittleHelperCard.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

LittleHelperCard::LittleHelperCard()
    : ItemEffect()
    , m_nAddTime(0)
    , m_nId(0)
    , m_nLittleHelpId(0)
{
}

LittleHelperCard::~LittleHelperCard()
{
}
