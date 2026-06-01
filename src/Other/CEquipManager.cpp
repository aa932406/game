#include "Other/CEquipManager.h"
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

class Player;
class GameService;

CEquipManager::CEquipManager()
    : m_IdLock(new Answer::RwLock())
    , m_rwEquipLock(new Answer::RwLock())
{
}

CEquipManager::~CEquipManager()
{
    delete m_IdLock;
    delete m_rwEquipLock;
}

void CEquipManager::OnEquipUpdated(const MemEquipVector* vAmulet)
{
    if (!vAmulet)
        return;

    for (MemEquipVector::const_iterator iter = vAmulet->begin(); iter != vAmulet->end(); ++iter)
    {
        Answer::RwLockWrGuard lock(m_rwEquipLock);
        const MemEquip& equip = *iter;
        m_mEquip[equip.id] = equip;
    }
}

void CEquipManager::SendPlayerEquipInfo(Player* pPlayer)
{
    if (!pPlayer)
        return;
    (void)pPlayer;
}

void CEquipManager::BroadcastEquipInfo(const MemEquip* equip)
{
    if (!equip)
        return;
    (void)equip;
}

int64_t CEquipManager::getEquipId(int32_t nServerId)
{
    (void)nServerId;
    return 0;
}
