#include "Other/Node.h"

Node::Node()
    : pos(0, 0)
    , inOpen(0)
    , inClose(0)
    , g(0)
    , h(0)
    , f(0)
{
}

Node::Node(const Position& position)
    : pos(position)
    , inOpen(0)
    , inClose(0)
    , g(0)
    , h(0)
    , f(0)
{
}
