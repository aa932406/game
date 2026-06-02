#ifndef _CEQUIPMANAGER_H_
#define _CEQUIPMANAGER_H_

#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <cstring>

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
    static CEquipManager* GetInstance();
    void CreateMemEquip(MemEquip* equip, int8_t connId, int32_t param1, int32_t itemId, int32_t sid, int64_t createrId, std::string* name, int32_t a8, int32_t a9, int32_t time, int32_t a11, int32_t a12);
    void SendPlayerEquipInfo(Player* pPlayer);
    void SendPlayerEquipInfo(Player* pPlayer, const MemEquipVector* vEquip);
    static bool CreateMemEquip(CEquipManager* mgr, MemEquip* equip, int8_t connId, int32_t param1, int32_t itemId, int64_t srcId, int8_t bind, int32_t endTime);
    static void DeleteMemEquip(CEquipManager* mgr, int8_t connId, int64_t srcId, int32_t reason);
    static void SendPlayerEquipInfo(CEquipManager* mgr, Player* p, const MemEquipVector* v) { if (mgr) mgr->SendPlayerEquipInfo(p, v); }
    static void UpdateMemEquip(CEquipManager* mgr, int8_t connId, MemEquip* equip, int32_t reason) { (void)mgr; (void)connId; (void)equip; (void)reason; }
    static void GetMemEquip(MemEquip* out, CEquipManager* mgr, int64_t srcId) { if (out) memset(out, 0, sizeof(MemEquip)); (void)mgr; (void)srcId; }
    void BroadcastEquipInfo(const MemEquip* equip);
    int64_t getEquipId(int32_t nServerId);

private:
    Answer::RwLock* m_IdLock;
    std::map<int64_t, MemEquip> m_mEquip;
    std::map<int, ServerNewId> m_mNewId;
    Answer::RwLock* m_rwEquipLock;
};

#endif // _CEQUIPMANAGER_H_
