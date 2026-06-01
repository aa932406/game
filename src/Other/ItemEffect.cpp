#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>

ItemEffect::~ItemEffect()
{
}

/*
// Note: In the original binary, operator delete was called after destructor.
// This is handled by the compiler when using 'delete' on the object.
void ItemEffect::operator delete(void* ptr)
{
    ::operator delete(ptr);
}
*/
