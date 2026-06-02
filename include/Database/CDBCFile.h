#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>

class CDBCFile {
public:
    struct FIELD {
        int32_t iValue;
        float fValue;
        const char* pString;
    };

    int _ConvertStringToVector(const char *strStrINTgSource, std::vector<std::string> *const vRet, const char *szKey, bool bOneOfKey, bool bIgnoreEmpty);
    CDBCFile();
    ~CDBCFile();
    bool OpenFromMemory(const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromMemoryImpl_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromMemoryImpl_Text(const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromTXT(const char *szFileName);
    void CreateIndex(int nColum, const char *szFileName);
    int32_t GetRecordsNum();
    int32_t GetFieldsNum();
    FIELD* Search_Posistion(int32_t row, int32_t col);

private:
    // TODO: member variables
};
