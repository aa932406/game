#include "Other/CLibraryItem.h"

CLibraryItem::CLibraryItem()
    : IsGet(0)
    , nProbability(0)
{
}

CLibraryItem::CLibraryItem(const CLibraryItem& a2)
    : IsGet(a2.IsGet)
    , nProbability(a2.nProbability)
    , Items(a2.Items)
{
}
