#include "Other/DaZheQuan.h"

DaZheQuan::DaZheQuan()
    : Index(0)
    , CurrencyType(0)
    , CurrencyValues(0)
{
}

DaZheQuan::DaZheQuan(const DaZheQuan& a2)
    : Index(a2.Index)
    , CurrencyType(a2.CurrencyType)
    , CurrencyValues(a2.CurrencyValues)
    , Items(a2.Items)
{
}
