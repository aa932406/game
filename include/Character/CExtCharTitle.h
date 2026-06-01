// CExtCharTitle.h
#ifndef CEXTCHARTITLE_H
#define CEXTCHARTITLE_H

#include "CExtSystemBase.h"
#include <map>

class CExtCharTitle : public CExtSystemBase
{
public:
    CExtCharTitle();
    virtual ~CExtCharTitle();
    
    virtual void Init(Player* pPlayer) override;
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onDressTitle(Answer::NetPacket* inPacket);
    int32_t onUnDressTitle(Answer::NetPacket* inPacket);
    
    bool dressTitle(int32_t nId);
    bool unDressTitle(int32_t nId);
    void SendTitleInfo();
    void AppendDressTitle(Answer::NetPacket* packet);
    void AddCharAttr();
    void InitTitle();
    void CheckAddTitle(int8_t type, int32_t param);
    void RemoveTitle(int32_t type, int32_t param);
    void autoDressTitle(int32_t nId);
    void HideActivityTitle(bool bCleanActivityTitle);
    bool SetActivityTitle(int8_t nPos, int32_t nTitle);
    int32_t GetDressTitleId();
    void OnLevelUp(int32_t nLevel);
    bool isTitleHide(int32_t nId);
    void hideTitle(int32_t nId);
    void unHideTitle(int32_t nId);
    
private:
    void initTitleMap();
    void resetTitleMap();
    void addAttr(const void* attrAddons);
    
    int32_t m_nDressTitle;
    int32_t m_nDressNormalTitle;
    int32_t m_nBattleRankTop;
    bool m_bShowActivityTitle;
    int64_t m_LastUpdateTick;
    void* m_vDress[3];
    int32_t m_vActivityTitle[3];
    std::map<int32_t, void*> m_mCharTitle;
};

#endif