// CExtOperateLimit.h
#ifndef CEXTOPERATELIMIT_H
#define CEXTOPERATELIMIT_H

#include "CExtSystemBase.h"
#include <map>
#include <list>

struct OperateLimit
{
    int32_t LimitId;
    int32_t LimitCount;
    int8_t IsChaned;
};

typedef std::map<int32_t, OperateLimit> OperateLimitMap;
typedef std::list<int32_t> Int32List;

class CExtOperateLimit : public CExtSystemBase
{
public:
    CExtOperateLimit();
    virtual ~CExtOperateLimit();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    void UpdateLimitCount(int32_t LimitId, int32_t LimitCount);
    void AddLimitCount(int32_t LimitId, int32_t LimitCount);
    static void AddLimitCount(CExtOperateLimit* self, int32_t LimitId, int32_t LimitCount) { if (self) self->AddLimitCount(LimitId, LimitCount); }
    void Reset(int32_t LimitId);
    void ResetRange(int32_t MinId, int32_t MaxId, int32_t DiffDay);
    int32_t GetLimitCount(int32_t LimitId);
    bool CheckIsLimitedForever(int32_t LimitId);
    
    void NotifyLimitInfo();
    void NotifyLimitInfo(const Int32List* rcList);
    
private:
    void checkMaintainCompensate();
    bool isNeedNotify(int32_t nLimitId);
    
    OperateLimitMap m_mOperateLimit;
};

#endif