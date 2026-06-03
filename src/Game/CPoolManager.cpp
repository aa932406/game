#include "common.h"
#include "Game/CPoolManager.h"

#include <cstring>
#include <algorithm>
#include <string>

// 前置声明所有需要注册的对象类型
class GameMsg;
class User;
class Player;
class CObjPet;
class Monster;
class Dungeon;
class MonsterDungeon;
class CDropItem;
class Trap;
class PlantDungeon;
class MonsterActivity;
class NpcActivity;
class PlantActivity;
class Trailer;
class CObjCarrier;
class CTerritory;
class CLittleHelper;

void CPoolManager::Init()
{
    Answer::Inifile ini;
    std::string file("./ServerConfig/Config/PoolConfig.ini");
    ini.parse(&file);

    // 注册所有对象池
    CPoolManager::regPool<GameMsg>(this, ini.getIntValue("GameMsg", "min"), ini.getIntValue("GameMsg", "max"));
    CPoolManager::regPool<User>(this, ini.getIntValue("User", "min"), ini.getIntValue("User", "max"));
    CPoolManager::regPool<Player>(this, ini.getIntValue("Player", "min"), ini.getIntValue("Player", "max"));
    CPoolManager::regPool<CObjPet>(this, ini.getIntValue("CObjPet", "min"), ini.getIntValue("CObjPet", "max"));
    CPoolManager::regPool<Monster>(this, ini.getIntValue("Monster", "min"), ini.getIntValue("Monster", "max"));
    CPoolManager::regPool<Dungeon>(this, ini.getIntValue("Dungeon", "min"), ini.getIntValue("Dungeon", "max"));
    CPoolManager::regPool<MonsterDungeon>(this, ini.getIntValue("MonsterDungeon", "min"), ini.getIntValue("MonsterDungeon", "max"));
    CPoolManager::regPool<CDropItem>(this, ini.getIntValue("CDropItem", "min"), ini.getIntValue("CDropItem", "max"));
    CPoolManager::regPool<Trap>(this, ini.getIntValue("Trap", "min"), ini.getIntValue("Trap", "max"));
    CPoolManager::regPool<PlantDungeon>(this, ini.getIntValue("PlantDungeon", "min"), ini.getIntValue("PlantDungeon", "max"));
    CPoolManager::regPool<MonsterActivity>(this, ini.getIntValue("MonsterActivity", "min"), ini.getIntValue("MonsterActivity", "max"));
    CPoolManager::regPool<NpcActivity>(this, ini.getIntValue("NpcActivity", "min"), ini.getIntValue("NpcActivity", "max"));
    CPoolManager::regPool<PlantActivity>(this, ini.getIntValue("PlantActivity", "min"), ini.getIntValue("PlantActivity", "max"));
    CPoolManager::regPool<Trailer>(this, ini.getIntValue("Trailer", "min"), ini.getIntValue("Trailer", "max"));
    CPoolManager::regPool<CObjCarrier>(this, ini.getIntValue("CObjCarrier", "min"), ini.getIntValue("CObjCarrier", "max"));
    CPoolManager::regPool<CTerritory>(this, ini.getIntValue("Territory", "min"), ini.getIntValue("Territory", "max"));
    CPoolManager::regPool<CLittleHelper>(this, ini.getIntValue("LittlerHelper", "min"), ini.getIntValue("LittlerHelper", "max"));
}
