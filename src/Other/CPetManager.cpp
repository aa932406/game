#include "Other/CPetManager.h"
#include "Network/NetPacket.h"

namespace Answer
{
    class RwLock
    {
    public:
        RwLock() {}
        ~RwLock() {}
    };

    class RwLockWrGuard
    {
    public:
        RwLockWrGuard(RwLock* lock) : m_lock(lock) {}
        ~RwLockWrGuard() {}
    private:
        RwLock* m_lock;
    };
}

class GameService;
class CfgData;
class CfgPetTable;
class CPoolManager;

CPetManager::CPetManager()
    : m_IdLock(new Answer::RwLock())
    , m_rwLock(new Answer::RwLock())
    , m_nLastTick(0)
{
}

CPetManager::~CPetManager()
{
    delete m_IdLock;
    delete m_rwLock;
}

int32_t CPetManager::GetPetRankIndex(PetId_t nPetId)
{
    Answer::RwLockWrGuard lock(m_rwLock);
    std::map<int64_t, int16_t>::iterator findIter = m_mPetRank.find(nPetId);
    if (findIter != m_mPetRank.end())
        return findIter->second;
    return 0;
}

CObjPet* CPetManager::CreateNewPet(int32_t nBaseId, Player* pCreater)
{
    if (!pCreater)
        return 0;
    (void)nBaseId;
    return 0;
}

PetId_t CPetManager::getPetId(int32_t nServerId)
{
    (void)nServerId;
    return 0;
}

void CPetManager::OnUpdatePetRank(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return;

    int32_t nCount = inPacket->readInt32();
    for (int32_t i = 0; i < nCount; ++i)
    {
        PetId_t nPetId = inPacket->readInt64();
        int16_t nIndex = inPacket->readInt16();
        {
            Answer::RwLockWrGuard lock(m_rwLock);
            m_mPetRank[nPetId] = nIndex;
        }
    }
}

void CPetManager::OnInitPetRank(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return;

    int32_t nCount = inPacket->readInt32();
    for (int32_t i = 0; i < nCount; ++i)
    {
        PetId_t nPetId = inPacket->readInt64();
        int16_t nIndex = inPacket->readInt16();
        {
            Answer::RwLockWrGuard lock(m_rwLock);
            m_mPetRank[nPetId] = nIndex;
        }
    }
}
