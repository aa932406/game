#include "Other/ChatValidate.h"

ChatValidate::ChatValidate()
    : SendPacket(0)
    , Time(0)
    , CharId(0)
    , Channel(0)
    , FamilyId(0)
    , SendCharId(0)
{
}

ChatValidate::ChatValidate(const ChatValidate& a2)
    : SendPacket(a2.SendPacket)
    , Time(a2.Time)
    , CharId(a2.CharId)
    , Channel(a2.Channel)
    , FamilyId(a2.FamilyId)
    , SendCharId(a2.SendCharId)
    , NeedSender(a2.NeedSender)
{
}
