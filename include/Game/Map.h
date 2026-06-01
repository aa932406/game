#ifndef _MAP_H_
#define _MAP_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <ctime>

#include "Common/CommonTypes.h"

class Player;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Position;
class Plant;
class Trap;
class Trailer;
class CObjPet;
class CDropItem;
class CDropItemGroup;
class CLittleHelper;
class CMapRunner;
class CWorldBoss;

namespace Answer {
    class NetPacket;
}

class Map
{
public:
    Map();
    ~Map();

    Position pixelToTile(int32_t px, int32_t py);
    Position tileToPixel(int32_t tx, int32_t ty);
    Direction pixelDirection(int32_t cx_0, int32_t cy, int32_t tx, int32_t ty);
    Direction tileDirection(int32_t cx_0, int32_t cy, int32_t tx, int32_t ty);
    Direction UnitDirection(Position * src, Position * tar);
    Position getAroundTile(int32_t tx, int32_t ty, Direction direction);
    Position getAroudFreeTileBuyDir(int32_t tx, int32_t ty, Direction direction);
    Position getAroudFreeTile(int32_t tx, int32_t ty);
    void init(CMapRunner *pRunner, CfgMap * cfgmap);
    void update();
    bool IsCrossMap();
    void ResetRefreshMonster();
    void updateGroupMonsterRevive(int32_t Now);
    int32_t canEnter(Player *player);
    bool SpecialSitRevive();
    bool CanAutoRevive();
    void onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster);
    bool OnSitRevive(Player *player);
    bool CanUsePet();
    bool CanStayInMap();
    int32_t GetAvgBattle();
    int64_t getTick();
    int32_t getNow();
    void getLocalNow(tm *__return_ptr retstr);
    void getTile(int32_t tx, int32_t ty);
    bool isWalkablePosition(int32_t tx, int32_t ty);
    bool inStack(int32_t tx, int32_t ty);
    Position getAreaByPos(Position * pos);
    int32_t getViewAreaCount(int32_t nSize);
    void broadcastToAreas(Answer::NetPacket *packet, PositionVector * areas);
    void sendSelfEnterArea(Player *player, Position * oldPos);
    void getAreaUnits(PositionVector * areas, UnitList * outUnits);
    void getAllObjects(ObjList * outObjs);
    void packageUnitEnterArea(int8_t connid, ObjList * inList);
    void packageUnitLeaveArea(int8_t connid, ObjList * inList);
    void checkAreaChange(Unit *pUnit, Position * oldPos);
    void addPlayer(Player *player, int32_t x, int32_t y);
    void removePlayer(Player *player, bool islogout);
    void GetNearestAliveEvilUnit(Monster *monster);
    void GetNearestAliveEnemySideUnit(Monster *monster);
    void GetNearestDropItem(CLittleHelper *pLittleHelper);
    void GetNearestAliveEnemySideUnit(CObjPet *Pet);
    void addMonster(Monster *monster, int32_t x, int32_t y);
    void addNpc(Npc *npc, int32_t x, int32_t y);
    void getNpc(EntityId_t id);
    void addDropItem(CDropItem *dropItem);
    bool HaveDropAndPlayer(Position Pos);
    void addDropItemGroup(CDropItemGroup * dropItemGroup);
    void getDropItem(EntityId_t dropid);
    void addPlant(Plant *plant);
    void getPlant(EntityId_t id);
    void removePlant(EntityId_t id);
    void AddLittleHelper(CLittleHelper *pLittleHelper);
    void removeLittleHelper(CLittleHelper *pLittleHelper);
    void addTrailer(Trailer *trailer, int32_t x, int32_t y);
    void removeTrailer(Trailer *trailer);
    void addPet(CObjPet *pet);
    void removePet(CObjPet *pet);
    void addTrap(Trap *trap);
    void removeTrap(Trap *trap);
    void getTrap(EntityId_t id);
    bool inSafeRegion(Position * pos);
    bool inRegion(Position * pos, CfgMapRegion * region);
    Position getRandomWalkablePosition();
    Position getRandomWalkablePositionInRegion(CfgMapRegion * region);
    Position getRandomWalkableAroundPosition(int32_t tx, int32_t ty);
    void sendEnterMap(Player *player, int32_t x, int32_t y);
    void sendNpcsInMap(Player *player);
    void sendTrailersInMap(Player *player);
    void sendTeamsInMap(Player *player);
    void broadcastIntoMap(StaticObj *pObj);
    void broadcastLeaveMap(StaticObj *pObj);
    void broadcast(Answer::NetPacket *inPacket);
    void broadcastAreaAround(Answer::NetPacket *inPacket, StaticObj *pObj);
    int32_t xAddHalfY(int32_t x, int32_t y);
    int32_t xSubHalfY(int32_t x, int32_t y);
    Position firstLinePosition(Position src, Position dest);
    bool inAreas(StaticObj *pObj, PositionVector * areas);
    bool checkPos(Position * pos, Position * area);
    void updatePlayers();
    bool IsSpecialBossMap();
    bool IsBeastShrine();
    bool IsMoLingHuanYu();
    bool IsCycleTower();
    bool IsKuangDong();
    bool IsWastelands();
    bool IsXinMoCun();
    bool IsXinMoMap();
    Position GetEnterPos();
    void updateMonsters();
    void updateNpcs();
    void updatePlants();
    void updateDropItems();
    void updateTraps();
    void updateTrailers();
    void SendStacksInMap(Player *player);
    void SendObjsInMap(Player *player);
    void appendStack(Answer::NetPacket *packet, PosList * posList);
    void AddStack(PosList * posList);
    void RemoveStack(PosList * posList);
    void removeStack(Position * pos);
    void removeNpc(EntityId_t npcid);
    void OnKillMonster(EntityId_t MonsterId);
    void AddContribution(int32_t Values, CURRENCY_CHANGE_REASON nOpWay);
    void delMonster(int32_t mid);
    void cleanActivityMonster(int32_t nActivityId);
    void OnStartGoblinRuQin(int32_t nActivityId, int32_t RefreshCount);
    void GoblinAddMonster(int32_t nActivityId, int32_t mid, int32_t mapId, int x, int y);
    void ActivityRefreshMonster(int32_t nActivityId, int32_t nTime, int32_t nIndex);
    bool HaveUnit(int32_t x, int32_t y);
    bool HaveMonster(int32_t x, int32_t y);
    Position GetDirTileByDistance(Position CurPos, int32_t Direc, int32_t distance);
    Position MoveOneTile(Position CurPos, int32_t Direc);
    void AllPlayerBackCity();
    int32_t GetMapPlayerSize();
    int32_t GetAliveBossCount(int32_t nLevel);
    int32_t GetMapPlayerAliveSize();
    bool NearNpc(int32_t NpcId, int32_t x, int32_t y, int32_t Diff);

private:
        std::list<CLittleHelper*> m_CLittleHelperList;
        int32_t m_IsStart;
        std::map<int, int> m_MonsterReviveMap;
        int8_t m_NeedUpdate;
        int64_t m_StartandEndTime;
        CfgMap* m_cfgmap;
        std::list<CDropItem*> m_dropItems;
        int32_t m_heightAreaCount;
        int32_t m_heightTileCount;
        tm m_lastLocalNow;
        int64_t m_lastUpdateTime;
        std::list<Position> m_lstStack;
        std::list<Monster*> m_monsters;
        std::list<Npc*> m_npcs;
        CMapRunner* m_pMapRunner;
        std::list<CObjPet*> m_pets;
        std::list<Plant*> m_plants;
        PlayerList m_players;
        std::list<Trailer*> m_trailers;
        std::list<Trap*> m_traps;
        int32_t m_widthAreaCount;
        int32_t m_widthTileCount;
};

#endif // _MAP_H_