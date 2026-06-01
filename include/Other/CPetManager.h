#ifndef _CPETMANAGER_H_
#define _CPETMANAGER_H_

#include <cstdint>
#include <map>

namespace Answer
{
    class NetPacket;
    class RwLock;
    class RwLockWrGuard;
}

struct ServerNewId
{
    int64_t nNextId;
    int64_t nMaxId;
};

typedef int64_t PetId_t;
class Player;
class CObjPet;

class CPetManager
{
public:
    CPetManager();
    ~CPetManager();

    int32_t GetPetRankIndex(PetId_t nPetId);
    CObjPet* CreateNewPet(int32_t nBaseId, Player* pCreater);
    PetId_t getPetId(int32_t nServerId);
    void OnUpdatePetRank(Answer::NetPacket* inPacket);
    void OnInitPetRank(Answer::NetPacket* inPacket);

private:
    Answer::RwLock* m_IdLock;
    std::map<int, ServerNewId> m_mNewId;
    std::map<int64_t, int16_t> m_mPetRank;
    int64_t m_nLastTick;
    Answer::RwLock* m_rwLock;
};

#endif // _CPETMANAGER_H_
