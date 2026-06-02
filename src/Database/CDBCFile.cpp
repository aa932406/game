#include "Database/CDBCFile.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <algorithm>

// ==================== 简单实用函数 ====================

const char* CDBCFile::_GetLineFromMemory(char* szLine, int nMaxLen, const char* pMemory, const char* pDeadEnd)
{
    // TODO: implement
    (void)szLine;
    (void)nMaxLen;
    (void)pMemory;
    (void)pDeadEnd;
    return nullptr;
}

int CDBCFile::_ConvertStringToVector(const char *strStrINTgSource, std::vector<std::string> *const vRet, const char *szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
    // TODO: implement from decompiled code
    (void)strStrINTgSource;
    (void)szKey;
    (void)bOneOfKey;
    (void)bIgnoreEmpty;
    if (!vRet)
        return 0;
    vRet->clear();
    return 0;
}

// ==================== 构造函数/析构函数 ====================

CDBCFile::CDBCFile()
    : m_ID(0)
    , m_pStringBuf(nullptr)
    , m_nIndexColum(-1)
    , m_pf(nullptr)
    , m_nRecordsNum(0)
    , m_nFieldsNum(0)
    , m_nStringBufSize(0)
{
}

CDBCFile::~CDBCFile()
{
    if (m_pf)
    {
        fclose(m_pf);
        m_pf = nullptr;
    }
    if (m_pStringBuf)
    {
        operator delete[](m_pStringBuf);
        m_pStringBuf = nullptr;
    }
}

// ==================== 文件打开函数 ====================

bool CDBCFile::OpenFromMemory(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
    if (!pMemory || !pDeadEnd)
        assert(pMemory && pDeadEnd);

    // Check for binary DBC magic number (0xDDDD0000 = -574895104 as signed int)
    int32_t magic;
    memcpy(&magic, pMemory, sizeof(magic));
    if (magic == -574895104)
        return this->OpenFromMemoryImpl_Binary(pMemory, pDeadEnd, szFileName);
    else
        return this->OpenFromMemoryImpl_Text(pMemory, pDeadEnd, szFileName);
}

bool CDBCFile::OpenFromMemoryImpl_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
    // TODO: implement from decompiled code
    (void)pMemory;
    (void)pDeadEnd;
    (void)szFileName;
    return false;
}

bool CDBCFile::OpenFromMemoryImpl_Text(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
    // TODO: implement from decompiled code
    (void)pMemory;
    (void)pDeadEnd;
    (void)szFileName;
    return false;
}

bool CDBCFile::OpenFromTXT(const char *szFileName)
{
    if (!szFileName)
        assert(szFileName);

    m_pf = fopen(szFileName, "rb");
    if (!m_pf)
        return false;

    fseek(m_pf, 0, SEEK_END);
    int nFileSize = ftell(m_pf);
    fseek(m_pf, 0, SEEK_SET);

    char* pMemory = new char[nFileSize + 1];
    if (!pMemory)
    {
        fclose(m_pf);
        m_pf = nullptr;
        return false;
    }

    fread(pMemory, 1, nFileSize, m_pf);
    pMemory[nFileSize] = 0;

    bool bRet = this->OpenFromMemory(pMemory, &pMemory[nFileSize + 1], szFileName);

    delete[] pMemory;
    return bRet;
}

void CDBCFile::CreateIndex(int nColum, const char *szFileName)
{
    // TODO: implement from decompiled code
    (void)nColum;
    (void)szFileName;
}

// ==================== 简单查询函数 ====================

int32_t CDBCFile::GetRecordsNum()
{
    return m_nRecordsNum;
}

int32_t CDBCFile::GetFieldsNum()
{
    return m_nFieldsNum;
}

CDBCFile::FIELD* CDBCFile::Search_Posistion(int32_t row, int32_t col)
{
    if (row < 0 || row >= m_nRecordsNum || col < 0 || col >= m_nFieldsNum)
        return nullptr;
    return &m_vDataBuf[row * m_nFieldsNum + col];
}
