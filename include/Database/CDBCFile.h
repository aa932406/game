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
    bool OpenFromMemory(const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromMemoryImpl_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromMemoryImpl_Text(const char *pMemory, const char *pDeadEnd, const char *szFileName);
    bool OpenFromTXT(const char *szFileName);
    void CreateIndex(int nColum, const char *szFileName);

private:
    // TODO: member variables
};
