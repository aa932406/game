#ifndef _CRANKMIRROR_H_
#define _CRANKMIRROR_H_

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

class CRankMirror
{
public:
    CRankMirror();
    ~CRankMirror();

    int32_t GetBattleIndex(CharId_t nCharId);
    int32_t GetLevelIndex(CharId_t nCharId);
    bool IsJobBattleFirst(CharId_t nCharId);
    int32_t GetWeiWangIndex(CharId_t nCharId);
    void OnSocialUpdateRank(Answer::NetPacket *inPacket);
    void resetBattleRank();
    void resetLevelRank();
    void resetWeiWangRank();
    void setBattleRank(CharId_t nCharId, int32_t nIndex, int8_t nJobFrist);
    void setLevelRank(CharId_t nCharId, int32_t nIndex);
    void setWeiWangRank(CharId_t nCharId, int32_t nIndex);

private:
        // TODO: 确认类型 m_lock
        // TODO: 确认类型 m_mPetRank
        // TODO: 确认类型 m_mPlayerRank
        int64_t m_nLastRefreshTime;
        // TODO: 确认类型 m_vBattleRank
        // TODO: 确认类型 m_vLevelRank
        // TODO: 确认类型 m_vWeiWangRank
};

#endif // _CRANKMIRROR_H_
