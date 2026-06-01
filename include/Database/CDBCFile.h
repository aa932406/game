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
    int _ConvertStringToVector(const char *strStrINTgSource, std::vector<std::string> *const vRet, const char *szKey, bool bOneOfKey, bool bIgnoreEmpty);
    CDBCFile();
    ~CDBCFile();
    bool OpenFromMemory(CDBCFile *const this, const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromMemoryImpl_Binary(CDBCFile *const this, const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromMemoryImpl_Text(CDBCFile *const this, const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromTXT(CDBCFile *const this, const char *szFileName);
    void CreateIndex(CDBCFile *const this, int nColum, const char *szFileName);

private:
    // TODO: member variables
};
（内容由AI生成，仅供参考）
