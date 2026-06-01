#include "Config/CfgSysMail.h"


CfgSysMail::CfgSysMail(const CfgSysMail& a2)
{
    id = a2.id;
    sender_name = a2.sender_name;
    title = a2.title;
    content = a2.content;
    item = a2.item;
}

CfgSysMail& CfgSysMail::operator=(const CfgSysMail& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        sender_name = a2.sender_name;
        title = a2.title;
        content = a2.content;
        item = a2.item;
    }
    return *this;
}
