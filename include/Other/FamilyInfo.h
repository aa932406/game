#ifndef _FAMILYINFO_H_
#define _FAMILYINFO_H_

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

class FamilyInfo
{
public:
    FamilyInfo();
    ~FamilyInfo();

    bool IsDeclareWarFamily(FamilyId_t nFamilyId);
    void CleanUp();
    void UnPackageData(Answer::NetPacket *packet);
};

#endif // _FAMILYINFO_H_
