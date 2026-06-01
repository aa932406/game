// CharLittlerHelper.h
#ifndef CHARLITTLERHELPER_H
#define CHARLITTLERHELPER_H

#include "CExtSystemBase.h"
#include <map>
#include <set>
#include <string>

struct ActLittleHelperInfo
{
    int32_t nId;
    int32_t nTimes;
    int32_t nSkin;
};

class CharLittlerHelper : public CExtSystemBase
{
public:
    CharLittlerHelper();
    virtual ~CharLittlerHelper();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t OnAutoWork();
    int32_t OnWork(int32_t nId);
    int32_t OnWork(Answer::NetPacket* inPacket);
    int32_t OnRest(Answer::NetPacket* inPacket);
    int32_t OnRest();
    int32_t AddLittlerHelper(int32_t nId, int32_t nAddTime);
    int32_t OnBuyLittlerHelper(Answer::NetPacket* inPacket);
    
    void SetTargetTile(int32_t tx, int32_t ty, Direction dir);
    void AddCharAttr();
    void SendCharLittlerhelper();
    bool CanPick(int8_t nClass, int32_t nId);
    void parsePickString(std::string* p_PickString);
    
    bool IsPickMoney();
    bool IsPickSYB();
    bool IsPickImte();
    bool IsPickEquip();
    bool IsPickSpecialEquip();
    bool IsPickShouHu();
    
    void* GetActLittleHelperInfo(int32_t nId);
    
private:
    bool IsFilterEquipGrade(int32_t Grade);
    bool IsFilterItem(int8_t nclass, int32_t nId);
    
    std::map<int32_t, ActLittleHelperInfo> m_ActMap;
    std::set<int32_t> m_EquipFilter;
    std::map<std::pair<int8_t, int32_t>, int32_t> m_ItemFilterMap;
    void* m_CLittleHelper;
    int32_t m_LittleHelperId;
    int8_t m_PickSetUp[6];
    bool m_NeedPick;
};

#endif