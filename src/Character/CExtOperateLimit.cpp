// CExtOperateLimit.cpp
#include "CExtOperateLimit.h"
#include "Game/GameService.h"
#include "Network/NetPacket.h"
#include "Game/Player.h"
#include "Answer/Singleton.h"
#include "Database/PlayerDBData.h"

CExtOperateLimit::CExtOperateLimit()
    : CExtSystemBase()
{
    m_mOperateLimit.clear();
}

CExtOperateLimit::~CExtOperateLimit()
{
}

void CExtOperateLimit::OnCleanUp()
{
    m_mOperateLimit.clear();
}

void CExtOperateLimit::OnUpdate(int64_t curTick)
{
}

void CExtOperateLimit::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtOperateLimit::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_mOperateLimit = dbData->m_OperateLimit.m_mOperateLimit;
        if (!CheckIsLimitedForever(37205))
        {
            AddLimitCount(37205, 2);
        }
        checkMaintainCompensate();
    }
}

void CExtOperateLimit::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData && m_pPlayer)
    {
        dbData->m_OperateLimit.m_mOperateLimit.clear();
        
        for (auto& pair : m_mOperateLimit)
        {
            if (pair.first > 1000)
            {
                OperateLimit stu;
                stu.LimitId = pair.second.LimitId;
                stu.LimitCount = pair.second.LimitCount;
                stu.IsChaned = pair.second.IsChaned;
                dbData->m_OperateLimit.m_mOperateLimit[stu.LimitId] = stu;
            }
        }
    }
}

void CExtOperateLimit::GetInterestsProtocol(ProcIdList* procList)
{
}

void CExtOperateLimit::UpdateLimitCount(int32_t LimitId, int32_t LimitCount)
{
    std::list<int32_t> updataList;
    
    auto it = m_mOperateLimit.find(LimitId);
    if (it != m_mOperateLimit.end())
    {
        if (it->second.LimitCount != LimitCount)
        {
            it->second.LimitCount = LimitCount;
            it->second.IsChaned = 1;
            updataList.push_back(LimitId);
        }
    }
    else
    {
        OperateLimit limit;
        limit.LimitId = LimitId;
        limit.LimitCount = LimitCount;
        limit.IsChaned = 1;
        m_mOperateLimit[LimitId] = limit;
        updataList.push_back(LimitId);
    }
    
    NotifyLimitInfo(&updataList);
}

void CExtOperateLimit::AddLimitCount(int32_t LimitId, int32_t LimitCount)
{
    if (LimitCount == 0) return;
    
    std::list<int32_t> updataList;
    
    auto it = m_mOperateLimit.find(LimitId);
    if (it != m_mOperateLimit.end())
    {
        it->second.LimitCount += LimitCount;
        it->second.IsChaned = 1;
    }
    else
    {
        OperateLimit limit;
        limit.LimitId = LimitId;
        limit.LimitCount = LimitCount;
        limit.IsChaned = 1;
        m_mOperateLimit[LimitId] = limit;
    }
    
    updataList.push_back(LimitId);
    NotifyLimitInfo(&updataList);
}

void CExtOperateLimit::Reset(int32_t LimitId)
{
    std::list<int32_t> updataList;
    
    auto it = m_mOperateLimit.find(LimitId);
    if (it != m_mOperateLimit.end() && it->second.LimitCount != 0)
    {
        it->second.LimitCount = 0;
        it->second.IsChaned = 1;
        updataList.push_back(it->second.LimitId);
    }
    
    NotifyLimitInfo(&updataList);
}

void CExtOperateLimit::ResetRange(int32_t MinId, int32_t MaxId, int32_t DiffDay)
{
    std::list<int32_t> updataList;
    std::list<OperateLimit> AddList;
    
    for (auto it = m_mOperateLimit.begin(); it != m_mOperateLimit.end(); ++it)
    {
        OperateLimit* limit = &it->second;
        if (limit->LimitCount && limit->LimitId >= MinId && limit->LimitId <= MaxId)
        {
            if (DiffDay == 1 && limit->LimitId > 2999 && limit->LimitId <= 3999)
            {
                OperateLimit stu;
                stu.LimitId = limit->LimitId + 1000;
                stu.LimitCount = limit->LimitCount;
                stu.IsChaned = 1;
                AddList.push_back(stu);
            }
            limit->LimitCount = 0;
            limit->IsChaned = 0;
            updataList.push_back(limit->LimitId);
        }
    }
    
    for (auto& stu : AddList)
    {
        m_mOperateLimit[stu.LimitId] = stu;
    }
    
    NotifyLimitInfo(&updataList);
}

int32_t CExtOperateLimit::GetLimitCount(int32_t LimitId)
{
    auto it = m_mOperateLimit.find(LimitId);
    if (it != m_mOperateLimit.end())
    {
        return it->second.LimitCount;
    }
    return 0;
}

bool CExtOperateLimit::CheckIsLimitedForever(int32_t LimitId)
{
    auto it = m_mOperateLimit.find(LimitId);
    if (it != m_mOperateLimit.end())
    {
        return it->second.LimitCount != 0;
    }
    return false;
}

void CExtOperateLimit::NotifyLimitInfo()
{
    if (!m_pPlayer) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x27AD);
    
    if (!packet) return;
    
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);  // 预留位置
    
    for (auto& pair : m_mOperateLimit)
    {
        OperateLimit* limit = &pair.second;
        if (isNeedNotify(limit->LimitId) || limit->LimitId == 1070)
        {
            Answer::NetPacket::writeInt32(packet, limit->LimitId);
            Answer::NetPacket::writeInt32(packet, limit->LimitCount);
            ++nCount;
        }
    }
    
    int32_t offset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, 0);
    Answer::NetPacket::writeInt32(packet, nCount);
    Answer::NetPacket::setWOffset(packet, offset);
    
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v6 = Player::getConnId(m_pPlayer);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v7, v6, GateIndex, packet);
}

void CExtOperateLimit::NotifyLimitInfo(const Int32List* rcList)
{
    if (!m_pPlayer || !rcList || rcList->empty()) return;
    
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x27AD);
    
    if (!packet) return;
    
    int32_t nCount = 0;
    Answer::NetPacket::writeInt32(packet, 0);  // 预留位置
    
    for (auto it = rcList->begin(); it != rcList->end(); ++it)
    {
        int32_t nLimitId = *it;
        if (isNeedNotify(nLimitId))
        {
            int32_t nLimitCount = 0;
            auto findIter = m_mOperateLimit.find(nLimitId);
            if (findIter != m_mOperateLimit.end())
            {
                nLimitCount = findIter->second.LimitCount;
            }
            Answer::NetPacket::writeInt32(packet, nLimitId);
            Answer::NetPacket::writeInt32(packet, nLimitCount);
            ++nCount;
        }
    }
    
    if (nCount > 0)
    {
        int32_t offset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setWOffset(packet, 0);
        Answer::NetPacket::writeInt32(packet, nCount);
        Answer::NetPacket::setWOffset(packet, offset);
        
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        int8_t v7 = Player::getConnId(m_pPlayer);
        GameService* v8 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(v8, v7, GateIndex, packet);
    }
    else
    {
        Answer::NetPacket::destroy(packet);
    }
}

void CExtOperateLimit::checkMaintainCompensate()
{
    if (!m_pPlayer) return;
    
    auto it = m_mOperateLimit.find(1070);
    if (it == m_mOperateLimit.end())
    {
        int32_t Time = Player::getCreateTime(m_pPlayer);
        // 获取补偿索引
        int32_t nIndex = 1;  // 实际需要从配置表获取
        
        OperateLimit stu;
        stu.LimitId = 1070;
        stu.LimitCount = nIndex;
        stu.IsChaned = 0;
        m_mOperateLimit[1070] = stu;
    }
}

bool CExtOperateLimit::isNeedNotify(int32_t nLimitId)
{
    // 需要通知客户端的LimitId范围
    if (nLimitId > 3000 && nLimitId <= 3998) return true;
    if (nLimitId > 5000 && nLimitId <= 5998) return true;
    if (nLimitId > 20000 && nLimitId <= 29998) return true;
    if (nLimitId > 6000 && nLimitId <= 6998) return true;
    if (nLimitId > 9000 && nLimitId <= 9998) return true;
    if (nLimitId > 6999 && nLimitId <= 7999) return true;
    if (nLimitId > 36999 && nLimitId <= 37199) return true;
    if (nLimitId > 8009 && nLimitId <= 8020) return true;
    if (nLimitId > 9999 && nLimitId <= 10100) return true;
    if (nLimitId > 8029 && nLimitId <= 8039) return true;
    if (nLimitId > 10100 && nLimitId <= 10200) return true;
    if (nLimitId > 1040 && nLimitId <= 1048) return true;
    
    // 特定ID范围
    if (nLimitId >= 1906 && nLimitId <= 1907) return true;
    if (nLimitId >= 1913 && nLimitId <= 1915) return true;
    if (nLimitId >= 1924 && nLimitId <= 1925) return true;
    if (nLimitId == 2005) return true;
    if (nLimitId >= 2013 && nLimitId <= 2016) return true;
    if (nLimitId >= 2019 && nLimitId <= 2028) return true;
    if (nLimitId >= 2032 && nLimitId <= 2039) return true;
    if (nLimitId >= 2045 && nLimitId <= 2050) return true;
    if (nLimitId == 2064) return true;
    if (nLimitId == 2067) return true;
    if (nLimitId >= 2070 && nLimitId <= 2073) return true;
    if (nLimitId >= 2082 && nLimitId <= 2086) return true;
    if (nLimitId >= 2100 && nLimitId <= 2115) return true;
    if (nLimitId >= 2118 && nLimitId <= 2124) return true;
    if (nLimitId >= 2129 && nLimitId <= 2132) return true;
    if (nLimitId == 37205) return true;
    if (nLimitId >= 37200 && nLimitId <= 37209) return true;
    if (nLimitId >= 37212 && nLimitId <= 37213) return true;
    if (nLimitId == 37401) return true;
    if (nLimitId >= 37501 && nLimitId <= 37506) return true;
    
    // 其他范围
    if (nLimitId == 4) return true;
    if (nLimitId >= 1028 && nLimitId <= 1034) return true;
    if (nLimitId == 1039) return true;
    if (nLimitId == 1051) return true;
    if (nLimitId >= 1063 && nLimitId <= 1066) return true;
    if (nLimitId >= 1075 && nLimitId <= 1076) return true;
    if (nLimitId >= 1082 && nLimitId <= 1083) return true;
    if (nLimitId >= 1115 && nLimitId <= 1119) return true;
    if (nLimitId >= 1127 && nLimitId <= 1132) return true;
    if (nLimitId >= 1147 && nLimitId <= 1148) return true;
    if (nLimitId >= 1150 && nLimitId <= 1157) return true;
    if (nLimitId >= 1160 && nLimitId <= 1164) return true;
    if (nLimitId >= 1403 && nLimitId <= 1413) return true;
    if (nLimitId >= 1601 && nLimitId <= 1608) return true;
    if (nLimitId >= 1615 && nLimitId <= 1637) return true;
    if (nLimitId == 1904) return true;
    
    return false;
}