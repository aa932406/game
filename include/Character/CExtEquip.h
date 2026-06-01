// CExtEquip.h
#ifndef CEXTEQUIP_H
#define CEXTEQUIP_H

#include "CExtSystemBase.h"
#include <list>
#include <map>
#include <string>

struct MemChrBag
{
    int32_t itemId;
    int8_t itemClass;
    int32_t itemCount;
    int8_t bind;
    int32_t endTime;
    int64_t srcId;
};

struct MemEquip
{
    int64_t id;
    int32_t base;
    int32_t star;
    int32_t Lucky;
    int64_t owner;
};

struct ItemData
{
    int32_t m_nId;
    int8_t m_nClass;
    int32_t m_nCount;
};

class CExtEquip : public CExtSystemBase
{
public:
    CExtEquip();
    virtual ~CExtEquip();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    virtual int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    
    // 装备操作
    int32_t onEquipDress(Answer::NetPacket* inPacket);
    int32_t onEquipUnDress(Answer::NetPacket* inPacket);
    int32_t onEquipExchangeStar(Answer::NetPacket* inPacket);
    int32_t onEquipUpStar(Answer::NetPacket* inPacket);
    int32_t onEquipUpPhase(Answer::NetPacket* inPacket);
    int32_t onEquipUpPos(Answer::NetPacket* inPacket);
    int32_t onEquipStrenGthen(Answer::NetPacket* inPacket);
    int32_t onRelieveBind(Answer::NetPacket* inPacket);
    int32_t onBackEquip(Answer::NetPacket* inPacket);
    
    // 墨符相关
    int32_t onMoFuHuiShou(Answer::NetPacket* inPacket);
    int32_t onMoFuDuiHuan(Answer::NetPacket* inPacket);
    int32_t OnXinMoEquipHuiShou(Answer::NetPacket* inPacket);
    
    // 宝石相关
    int32_t onGemOpenHole(Answer::NetPacket* inPacket);
    int32_t onGemAdd(Answer::NetPacket* inPacket);
    int32_t onGemRemove(Answer::NetPacket* inPacket);
    int32_t onGemLevelUp(Answer::NetPacket* inPacket);
    
    // 神耀相关
    int32_t onShenYaoPosLevelUp(Answer::NetPacket* inPacket);
    
    // 信息发送
    void SendEquipInfo();
    void SendGemInfo();
    void SendGemInfo(int8_t nPos, int8_t nSlot);
    void SendPosLevelInfo();
    void SendStrenGthenInfo();
    void SendShenYaoPosLevelInfo();
    
    // 属性相关
    void AddCharAttr();
    void addEquipAttr();
    void addGemAttr();
    void addSuitAttr();
    void addPosLevelAttr();
    void AddStrengthenAttr();
    void addShenYaoPosAttr();
    
    // 查询接口
    int32_t GetPosLevel(int8_t Pos);
    int32_t getStrenGthenLevel(int8_t Pos);
    bool IsAllPosTop();
    int32_t IsAllPosLevel();
    int32_t GetEquipAllGemLevel();
    int32_t GetMoFuSuitLevel();
    int32_t GetGemOpenLevel(int32_t EquipSlot);
    int32_t GetTeJieValue(int32_t nSlot);
    int32_t GetEquipType(int32_t Slot);
    int32_t GetEquipSlotPlace(int32_t nType);
    int32_t GetMaxPolishLevel(int32_t Slot);
    int32_t GetMaxRefiningLevel(int32_t Slot);
    int32_t GetLowestStar();
    int32_t GetEquipAllCount();
    int32_t GetEquipAllStar();
    int32_t GetEquipAllUpPosLevel();
    int32_t HaveLevelEquipCount(int32_t Level);
    int32_t HaveStarEquipCount(int32_t Star);
    int32_t HaveQualityGradeEquipCount(int8_t nQuality, int8_t nGrade);
    int32_t GetWingEquipCount3SuitId();
    int32_t GetWingEquipCount6SuitId();
    int32_t GetCount6SuitId();
    int32_t GetCount12SuitId();
    int32_t GetWeaponBless();
    int32_t GetAddGemCount(int32_t nLevel);
    int32_t GetQualityCount(int32_t nQuality);
    
    void SetWeaponBless(int32_t nBless);
    void checkEquipSuit(bool IsGongGao);
    void CheckGongMingSuitBuff();
    void updateEquipStar();
    void updateEquipGem();
    void broadcastItemCombi(int32_t ItemId, int8_t ItemClass);
    void EquipUpStarGongGao(MemEquip* Equip);
    void broadcastGemLevelUp(int32_t nBroadId, const std::string* pName, int64_t Cid, int32_t nGemId);
    
private:
    void clearGemInfo();
    std::string saveGemString();
    void loadGemString(const std::string* str);
    std::string savePosLevelString();
    void loadPosLevelString(const std::string* str);
    std::string saveStrengthenString();
    void LoadStrengthenString(const std::string* str);
    std::string saveShenYaoString();
    void LoadShenYaoString(const std::string* str);
    
    void addDirty(int32_t nSlot);
    void checkDirty();
    void clearDirty();
    bool sendDirty();
    bool onEquipExchange(int32_t nBagSlot, int8_t nEquipSlot);
    bool checkDressEquip(int32_t nBagSlot, int8_t nEquipSlot);
    MemChrBag* GetEquipSlot(int32_t nSlot);
    int32_t CheckSuitId(int32_t nSrcId, int32_t DesId);
    int32_t GetEquipSuitId(int32_t nId);
    int32_t checkGongMingSuit();
    void PackageShenYaoPosLevelUp(Answer::NetPacket* packet);
    void PackageGemInfo(Answer::NetPacket* packet);
    
    MemChrBag m_vMemEquip[93];
    int32_t m_vMemPosLevel[10];
    int32_t m_vMemStrengthen[10];
    int32_t m_vShenYaoEquipPos[10];
    int32_t m_vMemGem[10][4];
    std::list<int32_t> m_lstDirty;
    std::list<int32_t> m_EquipSuit;
    int32_t m_nEquipStar;
    int32_t m_nEquipGem;
    int32_t m_nGongMingSuitId;
    int32_t m_nGongMingSuitBuff;
};

#endif