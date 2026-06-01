#ifndef _ITEMEFFECTMANAGER_H_
#define _ITEMEFFECTMANAGER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;
class ItemEffect;

namespace Answer {
    class RwLock;
}

class ItemEffectManager
{
public:
    ItemEffectManager();
    ~ItemEffectManager();

    void init();
    static int32_t effect(ItemEffectManager* mgr, int32_t itemId, Player* src, Player* dst, int32_t* count) { (void)mgr; (void)itemId; (void)src; (void)dst; (void)count; return 0; }

private:
    std::map<int, ItemEffect*> m_itemEffects;
    Answer::RwLock* m_itemEffectsLock;
};

#endif // _ITEMEFFECTMANAGER_H_
