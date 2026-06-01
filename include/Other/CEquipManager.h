#ifndef _CEQUIPMANAGER_H_
#define _CEQUIPMANAGER_H_

#include <cstdint>
#include <map>
#include <vector>

#include "Other/MemEquip.h"

namespace Answer { class RwLock; class RwLockWrGuard; }

struct ServerNewId
{
    int64_t nNextId;
    int64_t nMaxId;
};

typedef std::vector<MemEquip> MemEquipVector;

class Player;

class CEquipManager
{
public:
    CEquipManager();
    ~CEquipManager();

    void OnEquipUpdated(const MemEquipVector* vAmulet);
    void SendPlayerEquipInfo(Player* pPlayer);
    void BroadcastEquipInfo(const MemEquip* equip);
    int64_t getEquipId(int32_t nServerId);

private:
    Answer::RwLock* m_IdLock;
    std::map<int64_t, MemEquip> m_mEquip;
    std::map<int, ServerNewId> m_mNewId;
    Answer::RwLock* m_rwEquipLock;
};

#endif // _CEQUIPMANAGER_H_
