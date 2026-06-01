// CExtSystemBase.h
#ifndef CEXTSYSTEMBASE_H
#define CEXTSYSTEMBASE_H

#include <cstdint>
#include <list>

class Player;
class PlayerDBData;
namespace Answer { class NetPacket; }

typedef uint16_t ProcId_t;
typedef std::list<ProcId_t> ProcIdList;

class CExtSystemBase
{
public:
    CExtSystemBase();
    virtual ~CExtSystemBase();
    
    virtual void Init(Player* pPlayer);
    virtual void OnCleanUp() = 0;
    virtual void OnUpdate(int64_t curTick) = 0;
    virtual void OnDaySwitch(int32_t nDiffDays) = 0;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) = 0;
    virtual void OnSaveToDB(PlayerDBData* dbData) = 0;
    virtual void GetInterestsProtocol(ProcIdList* procList) = 0;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket);
    virtual void InitSystem();
    
protected:
    Player* m_pPlayer;
};

#endif