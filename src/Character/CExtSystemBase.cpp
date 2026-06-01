// CExtSystemBase.cpp
#include "CExtSystemBase.h"

CExtSystemBase::CExtSystemBase()
{
    m_pPlayer = nullptr;
}

void CExtSystemBase::Init(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}