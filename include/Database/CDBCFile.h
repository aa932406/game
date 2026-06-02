#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <cstring>
#include <unordered_map>

class CDBCFile {
public:
    struct FIELD {
        int32_t iValue;
        float fValue;
        const char* pString;
    };

    enum FIELD_TYPE {
        T_INT = 0,
        T_FLOAT = 1,
        T_STRING = 2,
    };

    struct FILE_HEAD {
        int32_t m_Identify;
        int32_t m_nRecordsNum;
        int32_t m_nFieldsNum;
        int32_t m_nStringBlockSize;
    };

    typedef std::vector<FIELD_TYPE> FILEDS_TYPE;

    static int _ConvertStringToVector(const char *strStrINTgSource, std::vector<std::string> *const vRet, const char *szKey, bool bOneOfKey, bool bIgnoreEmpty);
    static const char* _GetLineFromMemory(char* szLine, int nMaxLen, const char* pMemory, const char* pDeadEnd);

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
    std::vector<FIELD_TYPE> m_theType;
    std::vector<FIELD> m_vDataBuf;
    std::unordered_map<int, FIELD*> m_hashIndex;
    int m_ID;
    char* m_pStringBuf;
    int m_nIndexColum;
    FILE* m_pf;
    int m_nRecordsNum;
    int m_nFieldsNum;
    int m_nStringBufSize;
};
