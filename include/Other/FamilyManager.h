#ifndef _FAMILYMANAGER_H_
#define _FAMILYMANAGER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
class FamilyDungeonStu;
class Dungeon;

class FamilyManager
{
public:
    FamilyManager();
    ~FamilyManager();

    void Init(int32_t line);
    void OnUpdateFamilyInfo(Answer::NetPacket *inPacket);
    void OnInitFamilyInfo(Answer::NetPacket *inPacket);
    void addFamilyInfo(FamilyInfo * info);
    void updateFamilyInfo(FamilyInfo * info);
    void deleteFamilyInfo(FamilyId_t nFamilyId);
    int8_t GetTerritoryState(FamilyId_t nFamilyId);
    int32_t GetMedlLevel(FamilyId_t nFamilyId);
    Int64List GetTop10Family();
    void SaveFamilyDungeonStuMap(FamilyId_t nFamilyId);
    FamilyDungeonStu* GetFamilyDungeonInfo(FamilyId_t nFamilyId);
    Dungeon* GetFamilyDungeon(FamilyId_t nFamilyId);
    void DeleteFamilyDungeon(FamilyId_t nFamilyId);
    void SetFamilyDungeonHard(FamilyId_t nFamilyId, int32_t Hard);

private:
        // TODO: 确认类型 m_FamilyDungeonMap
        // TODO: 确认类型 m_FamilyDungeonStuMap
        // TODO: 确认类型 m_Lock
        // TODO: 确认类型 m_mFamilyInfo
};

#endif // _FAMILYMANAGER_H_
