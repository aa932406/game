#ifndef _CHARLITTLERHELPER_H_
#define _CHARLITTLERHELPER_H_

#include "Character/CExtSystemBase.h"
#include <map>
#include <set>
#include <string>

class ActLittleHelperInfo
{
public:
    int32_t nId;
    int32_t nTimes;
};

class CLittleHelper;

class CharLittlerHelper : public CExtSystemBase
{
public:
    CharLittlerHelper();
    virtual ~CharLittlerHelper();

    void OnLoadFromDB(const PlayerDBData* const dbData) override;
    void OnSaveToDB(PlayerDBData* const dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    int32_t OnAutoWork();
    int32_t OnWork(int32_t nId);
    int32_t OnWork(Answer::NetPacket* inPacket);
    void SetTargetTile(int32_t tx, int32_t ty, Direction dir);
    void AddCharAttr();
    int32_t OnRest(Answer::NetPacket* inPacket);
    int32_t OnRest();
    int32_t AddLittlerHelper(int32_t nId, int32_t nAddTime);
    int32_t OnBuyLittlerHelper(Answer::NetPacket* inPacket);
    ActLittleHelperInfo* GetActLittleHelperInfo(int32_t nId);
    void SendCharLittlerhelper();
    bool CanPick(int8_t nClass, int32_t nId);
    bool IsFilterEquipGrade(int32_t Grade);
    bool IsFilterItem(int8_t nclass, int32_t nId);
    void parsePickString(std::string* p_PickString);

private:
    int32_t m_LittleHelperId;
    CLittleHelper* m_CLittleHelper;
    std::map<int, ActLittleHelperInfo> m_ActMap;
    std::set<int> m_EquipFilter;
    std::map<std::pair<signed char, int>, int> m_ItemFilterMap;
};

#endif // _CHARLITTLERHELPER_H_
