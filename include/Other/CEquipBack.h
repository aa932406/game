#ifndef _CEQUIPBACK_H_
#define _CEQUIPBACK_H_
#include <cstdint>
#include <string>
#include <vector>
class CEquipBack {
public:
    CEquipBack();
    virtual ~CEquipBack();
    void UpdateEquipRecord(int32_t nId, int8_t nType, int32_t nTime, std::string* p_name) { (void)nId; (void)nType; (void)nTime; (void)p_name; }
    void UpdateEquipBackInfo(int32_t nId, int8_t nType, int32_t nCount, std::string* p_name) { (void)nId; (void)nType; (void)nCount; (void)p_name; }
};
#endif
