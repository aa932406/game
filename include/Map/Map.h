#pragma once
#include <cstdint>
#include <list>
#include <map>
#include <vector>

// 类型定义 (本地化，避免与 CfgData.h 冲突)
struct Position;
typedef int64_t EntityId_t;

// 方向枚举 (仅在 Common/CommonTypes.h 未定义时定义)
#ifndef DIRECTION_ENUM_DEFINED
#define DIRECTION_ENUM_DEFINED
enum class Direction
{
    DOWN        = 0,
    DOWN_RIGHT  = 1,
    RIGHT       = 2,
    UP_RIGHT    = 3,
    UP          = 4,
    UP_LEFT     = 5,
    LEFT        = 6,
    DOWN_LEFT   = 7,
};
#endif

// 货币变化原因 (仅在 Common/CommonTypes.h 未定义时定义)
#ifndef CURRENCY_CHANGE_REASON_DEFINED
#define CURRENCY_CHANGE_REASON_DEFINED
enum class CURRENCY_CHANGE_REASON
{
    CCR_NONE = 0,
    GCR_BUILD_TOWER,
    GCR_BUY_RANDOM_DUNGEON_TIMES,
    GCR_DUNGEON_DOUBLE_REWARD,
    GCR_DUNGEON_GUWU,
    GCR_DUNGEON_REWARD,
    GCR_DUNGEON_SUMMON,
    GCR_XINMO_SIT_REVIVE,
    GCR_YJSK_GUESS_SIZE,
    MCR_DUNGEON_REWARD,
};
#endif

// 容器类型 (用于方法签名)
typedef std::list<Player*>      PlayerList;
typedef std::list<Unit*>        UnitList;
typedef std::list<StaticObj*>   ObjList;
typedef std::vector<Unit*>      UnitVector;
typedef std::vector<Position>   PositionVector;
typedef std::list<Position>     PosList;

namespace Answer {
    class NetPacket;
}

class CfgActiveSkill;
class CMapRunner;
class CfgMap;
class CfgMapRegion;
class CfgActiveSkill;
class Player;
class Monster;
class Npc;
class Plant;
class Trap;
class Trailer;
class CObjPet;
class CDropItem;
class CDropItemGroup;
class CLittleHelper;
class StaticObj;
class Unit;
class Tile;

class Map
{
public:
    Map();
    virtual ~Map();
    
    static Direction UnitDirection(const Position &src, const Position &tar);
    static Position getAroundTile(int32_t tx, int32_t ty, Direction direction);
    static Position pixelToTile(int32_t px, int32_t py);
    static Direction pixelDirection(int32_t cx, int32_t cy, int32_t tx, int32_t ty);
    static Position tileToPixel(int32_t tx, int32_t ty);
    static Direction tileDirection(int32_t cx, int32_t cy, int32_t tx, int32_t ty);
    
    virtual void init(CMapRunner *pRunner, const CfgMap *cfgmap);
    virtual void update();
    virtual bool canEnter(Player *player);
    virtual bool CanEnter(Player* player);
    virtual bool OnSitRevive(Player *player);
    virtual bool SpecialSitRevive();
    virtual bool SpecialSitRevive(Player* player);
    virtual bool CanSitRevive(Player* player);
    virtual bool CanAutoRevive();
    virtual int32_t GetReive(Player* player);
    virtual int32_t GetPkMode();
    virtual void Update(int64_t curTick);
    virtual bool CanUsePet();
    virtual bool CanStayInMap();
    virtual int32_t GetAvgBattle();
    virtual int32_t getNow();
    virtual int64_t getTick();
    virtual void onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster);
    
    int32_t GetRunnerId();
    virtual bool IsActivityMap() { return false; }

    bool IsCrossMap() const;
    bool IsSpecialBossMap() const;
    bool IsBeastShrine() const;
    bool IsMoLingHuanYu() const;
    bool IsCycleTower() const;
    bool IsKuangDong() const;
    bool IsWastelands() const;
    bool IsXinMoCun() const;
    bool IsXinMoMap() const;
    bool inStack(int32_t tx, int32_t ty) const;
    bool checkPos(const Position &pos, const Position &area) const;
    int32_t getViewAreaCount(int32_t nSize) const;
    
    void ResetRefreshMonster();
    void updateGroupMonsterRevive(int32_t Now);
    
    Position getAroudFreeTileBuyDir(int32_t tx, int32_t ty, Direction direction);
    Position getAroudFreeTile(int32_t tx, int32_t ty);
    bool isWalkablePosition(int32_t tx, int32_t ty);
    Position getAreaByPos(const Position &pos);
    void getViewAreas(const Position &pos, PositionVector &outAreas, int32_t nSize);
    void getNewAreas(const Position &oldPos, const Position &newPos, PositionVector &outAreas, int32_t nSize);
    void broadcastToAreas(Answer::NetPacket *packet, const PositionVector &areas);
    void sendSelfEnterArea(Player *player, const Position &oldPos);
    void getAreaPlayer(PlayerList &outPlayers, const Position &pos, int32_t nRang);
    void getAreaUnits(const PositionVector &areas, UnitList &outUnits);
    void getAllObjects(ObjList &outObjs);
    void getAreaObjects(Player *launcher, const PositionVector &areas, ObjList &outObjs, bool bAll);
    void getAreaTargets(Unit *launcher, const Position &pos, const CfgActiveSkill *pCfg, UnitList &outUnits);
    Answer::NetPacket *packageUnitEnterArea(int8_t connid, const ObjList &inList);
    Answer::NetPacket *packageUnitLeaveArea(int8_t connid, const ObjList &inList);
    void checkAreaChange(Unit *pUnit, const Position &oldPos);
    Unit *GetNearestAliveEvilUnit(Monster *monster);
    Unit *GetNearestAliveEnemySideUnit(Monster *monster);
    CDropItem *GetNearestDropItem(CLittleHelper *pLittleHelper);
    void addMonster(Monster *monster, int32_t x, int32_t y);
    void addNpc(Npc *npc, int32_t x, int32_t y);
    Npc *getNpc(EntityId_t id);
    void addDropItem(CDropItem *dropItem);
    bool HaveDropAndPlayer(Position Pos);
    void addDropItemGroup(const CDropItemGroup &dropItemGroup);
    CDropItem *getDropItem(EntityId_t dropid);
    void addPlant(Plant *plant);
    Plant *getPlant(EntityId_t id);
    void removePlant(EntityId_t id);
    void AddLittleHelper(CLittleHelper *pLittleHelper);
    void removeLittleHelper(CLittleHelper *pLittleHelper);
    void addTrailer(Trailer *trailer, int32_t x, int32_t y);
    void removeTrailer(Trailer *trailer);
    void addPet(CObjPet *pet);
    void removePet(CObjPet *pet);
    void addTrap(Trap *trap);
    void removeTrap(Trap *trap);
    Trap *getTrap(EntityId_t id);
    
    bool inSafeRegion(const Position &pos);
    bool inRegion(const Position &pos, const CfgMapRegion &region);
    Position getRandomWalkablePosition();
    Position getRandomWalkablePositionInRegion(const CfgMapRegion &region);
    Position getRandomWalkableAroundPosition(int32_t tx, int32_t ty);
    Position getNearestWalkableAroundPosition(const Position &tarPos, const Position &srcPos);
    
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
    bool inAreas(StaticObj *pObj, const PositionVector &areas);
    
    void updatePlayers();
    void updateMonsters();
    void updateNpcs();
    void updatePlants();
    void updateDropItems();
    void updateTraps();
    void updateTrailers();
    void SendStacksInMap(Player *player);
    void SendObjsInMap(Player *player);
    void appendStack(Answer::NetPacket *packet, const PosList &posList);
    void AddStack(const PosList &posList);
    void RemoveStack(const PosList &posList);
    void removeStack(const Position &pos);
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
    
    Position GetEnterPos();
    Tile *getTile(int32_t tx, int32_t ty);
    tm getLocalNow();
    
    UnitVector getSkillTargets(Unit *launcher, Position pos, const CfgActiveSkill *pCfg);
    bool checkSkillTarget(Unit *launcher, Position pos, Unit *target, const CfgActiveSkill *pCfg);

protected:
    CfgMap* m_cfgmap;
    CMapRunner *m_pMapRunner;
    std::list<Player *> m_players;
    std::list<CObjPet *> m_pets;
    std::list<Monster *> m_monsters;
    std::list<Npc *> m_npcs;
    std::list<Trailer *> m_trailers;
    std::list<CDropItem *> m_dropItems;
    std::list<Plant *> m_plants;
    std::list<Trap *> m_traps;
    std::list<CLittleHelper *> m_CLittleHelperList;
    std::list<Position> m_lstStack;
    std::map<int, int> m_MonsterReviveMap;
    struct { char _[0x38]; } m_lastLocalNow;
    int32_t m_NeedUpdate;
};
