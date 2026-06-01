#include "Other/FindNode.h"

FindNode::FindNode()
    : parent(0, 0)
{
}

FindNode::FindNode(const FindNode& a2)
    : parent(a2.parent)
{
}
