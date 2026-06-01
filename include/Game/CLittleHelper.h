#ifndef _CLITTLEHELPER_H_
#define _CLITTLEHELPER_H_

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

class CLittleHelper
{
public:
    CLittleHelper();
    ~CLittleHelper();

    void reset();
    bool refresh();
    CharId_t getOwner();
    bool appendInfo(Answer::NetPacket *packet);
    void init(Player *Player, int32_t LittleHelperId, int32_t Clothes);
    void AddAttr();
    void EnterMap();
    void LeaveMap();
    void OnLogout();
    void CheckSwitchMap();
    void ChangeClothe(int32_t ClotheId);
    void ChangeLittleHelperId(int32_t LittleHelperId, int32_t ClotheId);
    Position getFollowTile(int32_t tx, int32_t ty, Direction dir);
    void SynsAreaAround();
    void updateState();
    void checkFarAway();
    void ResetTargetTile(int32_t tx, int32_t ty, Direction dir);
    void onStand();
    void ChangeTarget();
    void onFollow();
    void onWalkPick();
    void onPick();

    void OnWork();
    void OnRest();
    bool IsWork();
    int32_t OnGetLittleHelperId() const;

private:
        // TODO: 确认类型 m_Clothes
        int64_t m_LastTick;
        int32_t m_LittleHelperId;
        int64_t m_PickTick;
        int32_t m_TarDropId;
        int32_t m_WorkState;
        // TODO: 确认类型 m_nStandPlace
        int32_t m_nState;
        // TODO: 确认类型 m_pMap
        // TODO: 确认类型 m_pPlayer
};

#endif // _CLITTLEHELPER_H_