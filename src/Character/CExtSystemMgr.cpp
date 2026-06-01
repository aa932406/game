// CExtSystemMgr.cpp
#include "CExtSystemMgr.h"

CExtSystemMgr::CExtSystemMgr()
{
    m_mapProcHandler.clear();
    m_lstExtSystems.clear();
}

CExtSystemMgr::~CExtSystemMgr()
{
}

void CExtSystemMgr::Register(CExtSystemBase* pExtSystem)
{
    if (!pExtSystem) return;
    
    m_lstExtSystems.push_back(pExtSystem);
    
    ProcIdList procList;
    pExtSystem->GetInterestsProtocol(&procList);
    
    for (auto it = procList.begin(); it != procList.end(); ++it)
    {
        registProtocolHandler(*it, pExtSystem);
    }
}

void CExtSystemMgr::registProtocolHandler(ProcId_t procId, CExtSystemBase* pHanlder)
{
    if (pHanlder)
    {
        m_mapProcHandler[procId] = pHanlder;
    }
}

void CExtSystemMgr::OnLoadFromDB(const PlayerDBData* dbData)
{
    for (auto it = m_lstExtSystems.begin(); it != m_lstExtSystems.end(); ++it)
    {
        if (*it) (*it)->OnLoadFromDB(dbData);
    }
}

void CExtSystemMgr::OnSaveToDB(PlayerDBData* dbData)
{
    for (auto it = m_lstExtSystems.begin(); it != m_lstExtSystems.end(); ++it)
    {
        if (*it) (*it)->OnSaveToDB(dbData);
    }
}

void CExtSystemMgr::OnUpdate(int64_t curTick)
{
    for (auto it = m_lstExtSystems.begin(); it != m_lstExtSystems.end(); ++it)
    {
        if (*it) (*it)->OnUpdate(curTick);
    }
}

void CExtSystemMgr::InitSystem()
{
    for (auto it = m_lstExtSystems.begin(); it != m_lstExtSystems.end(); ++it)
    {
        if (*it) (*it)->InitSystem();
    }
}

void CExtSystemMgr::OnDaySwitch(int32_t nDiffDays)
{
    for (auto it = m_lstExtSystems.begin(); it != m_lstExtSystems.end(); ++it)
    {
        if (*it) (*it)->OnDaySwitch(nDiffDays);
    }
}

void CExtSystemMgr::OnCleanUp()
{
    for (auto it = m_lstExtSystems.begin(); it != m_lstExtSystems.end(); ++it)
    {
        if (*it) (*it)->OnCleanUp();
    }
    m_lstExtSystems.clear();
    m_mapProcHandler.clear();
}

void CExtSystemMgr::Unregister(CExtSystemBase* pExtSystem)
{
    if (!pExtSystem) return;

    m_lstExtSystems.remove(pExtSystem);

    ProcIdList procList;
    pExtSystem->GetInterestsProtocol(&procList);

    for (auto it = procList.begin(); it != procList.end(); ++it)
    {
        m_mapProcHandler.erase(*it);
    }
}

CExtSystemBase* CExtSystemMgr::GetHandler(ProcId_t procId)
{
    auto it = m_mapProcHandler.find(procId);
    if (it != m_mapProcHandler.end())
    {
        return it->second;
    }
    return nullptr;
}

int32_t CExtSystemMgr::DispatchProtocol(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return -1;

    auto it = m_mapProcHandler.find(nProcId);
    if (it != m_mapProcHandler.end() && it->second)
    {
        return it->second->DispatchNetDatas(nProcId, inPacket);
    }
    return -1;
}

void CExtSystemMgr::GetInterestsProtocol(ProcIdList* procList)
{
}