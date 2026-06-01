#include "Other/GameMsg.h"

void GameMsg::reset()
{
    msgcode = GameMsgCode::GMC_UNKNOWN;
    ptr1 = nullptr;
    ptr2 = nullptr;
    ptr3 = nullptr;
    iparam1 = 0;
    iparam2 = 0;
    effecttick = 0;
}
