#ifndef _CFABAO_H_
#define _CFABAO_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Common/CommonTypes.h"

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

// 法宝数据结构
struct FaBaoInfo
{
    int32_t FaBaoLevel;
    int32_t FaBaoRes;
};

class CFaBao
{
public:
    CFaBao();
    ~CFaBao();

    void OnCleanUp();
    void OnLoadFromDB(const PlayerDBData *dbData);
    void OnSaveToDB(PlayerDBData *dbData);
    void GetInterestsProtocol(ProcIdList *procList);
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket);
    int32_t onAskFaBaoInfo(Answer::NetPacket *inPacket);
    int32_t onBuyFaBaoRes(Answer::NetPacket *inPacket);
    int32_t onFaBaoPeiYang(Answer::NetPacket *inPacket);
    void broadcastLevelUp(int8_t Type, int32_t GongGaoId);
    void AddPlayerAttr();
    void sendFaBaoInfo(int8_t nType);
    void SendAllFaBaoInfo();
    void AddFaBaoRes(int8_t nType, int32_t Values);
    int32_t GetFaBaoLevel(int8_t Type);

    static bool IsFaBaoType(int8_t nType);

private:
    Player *m_pPlayer;
    FaBaoInfo m_FaBao[4];    // 4种类型的法宝数据
};

#endif // _CFABAO_H_