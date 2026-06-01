#include "Database/CDBCFile.h"

int CDBCFile::_ConvertStringToVector(const char *strStrINTgSource, std::vector<std::string> *const vRet, const char *szKey, bool bOneOfKey, bool bIgnoreEmpty)
{


    std::string strTemp; // [rsp+20h] [rbp-50h] BYREF
    std::string strItem; // [rsp+30h] [rbp-40h] BYREF
    std::string strSrc; // [rsp+40h] [rbp-30h] BYREF

    size_t nLeft; // [rsp+50h] [rbp-20h]
    size_t nRight; // [rsp+58h] [rbp-18h]

    std::vector<std::string>::clear(vRet);

    std::string::string(&strSrc, strStrINTgSource);
    std::allocator<char>::~allocator(&v17);
    if ( (unsigned __int8)std::string::empty(&strSrc) )
    {
    v5 = 0;
    }
    else
    {
    nLeft = 0;
    if ( bOneOfKey )
    nRight = std::string::find_first_of(&strSrc, szKey, 0);
    else
    nRight = std::string::find(&strSrc, szKey, 0);
    if ( nRight == -1 )
    nRight = std::string::length(&strSrc);
    do
    {
    std::string::substr(&strItem, (unsigned __int64)&strSrc, nLeft);
    if ( std::string::length(&strItem) || !bIgnoreEmpty )
    std::vector<std::string>::push_back(vRet, &strItem);
    v7 = std::string::length(&strSrc);
    if ( v7 == nRight )
    {
    v8 = 0;
    }
    else
    {
    if ( bOneOfKey )
    v9 = 1;
    else
    v9 = strlen(szKey);
    nLeft = nRight + v9;
    if ( bOneOfKey )
    {
    std::string::substr(&strTemp, (unsigned __int64)&strSrc, nLeft);
    nRight = std::string::find_first_of(&strTemp, szKey, 0);
    if ( nRight != -1 )
    nRight += nLeft;
    std::string::~string(&strTemp);
    }
    else
    {
    nRight = std::string::find(&strSrc, szKey, nLeft);
    }
    if ( nRight == -1 )
    nRight = std::string::length(&strSrc);
    v8 = 1;
    }
    std::string::~string(&strItem);
    }
    while ( v8 );
    v5 = std::vector<std::string>::size(vRet);
    }
    std::string::~string(&strSrc);
    return v5;
}

CDBCFile::CDBCFile()
{
    /* vcall: this->_vptr_CDBCFile = (int (**)(...))off_8F6CB0; */
    this->m_theType.vector();
    this->m_vDataBuf.vector();
    __gnu_cxx::hash_map<int,CDBCFile::FIELD *,__gnu_cxx::hash<int>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::hash_map(&this->m_hashIndex);
    this->m_ID = id;
    this->m_pStringBuf = 0;
    this->m_nIndexColum = -1;
    this->m_pf = 0;
}

CDBCFile::~CDBCFile()
{
    /* vcall: this->_vptr_CDBCFile = (int (**)(...))off_8F6CB0; */
    if ( this->m_pf )
    {
    fclose(this->m_pf);
    this->m_pf = 0;
    }
    if ( this->m_pStringBuf )
    {
    operator delete[](this->m_pStringBuf);
    this->m_pStringBuf = 0;
    }
    __gnu_cxx::hash_map<int,CDBCFile::FIELD *,__gnu_cxx::hash<int>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::~hash_map(&this->m_hashIndex);
    std::vector<CDBCFile::FIELD>::~vector(&this->m_vDataBuf);
    std::vector<CDBCFile::FIELD_TYPE>::~vector(&this->m_theType);
}

bool CDBCFile::OpenFromMemory(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{
    if ( !pMemory || !pDeadEnd )
    __assert_fail(
    "pMemory && pDeadEnd",
    "DBCFile.cpp",
    0x96u,
    "bool CDBCFile::OpenFromMemory(const char*, const char*, const char*)");
    if ( (unsigned __int64)(pDeadEnd - pMemory) > 0xF && *(_DWORD *)pMemory == -574895104 )
    return this->OpenFromMemoryImpl_Binary(pMemory, pDeadEnd, szFileName);
    else
    return this->OpenFromMemoryImpl_Text(pMemory, pDeadEnd, szFileName);
}

bool CDBCFile::OpenFromMemoryImpl_Binary(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{

    const char *v5; // rbx

    unsigned int *v7; // rax
    const char *v8; // rbx


    int FieldsNum; // eax

    int pDeadEnda; // [rsp+8h] [rbp-78h]
    std::vector<unsigned int> vFieldType; // [rsp+20h] [rbp-60h] BYREF
    CDBCFile::FILE_HEAD theHead; // [rsp+40h] [rbp-40h] BYREF
    CDBCFile::FIELD __x; // [rsp+50h] [rbp-30h] BYREF
    int i; // [rsp+64h] [rbp-1Ch]
    int j; // [rsp+68h] [rbp-18h]
    int i_0; // [rsp+6Ch] [rbp-14h]

    pDeadEnda = (int)pDeadEnd;
    memcpy(&theHead, pMemory, sizeof(theHead));
    if ( theHead.m_Identify != -574895104 )
    return 0;
    if ( 4 * (2 * theHead.m_nRecordsNum * (__int64)theHead.m_nFieldsNum + theHead.m_nFieldsNum + 4)
    + theHead.m_nStringBlockSize > (unsigned __int64)(unsigned int)(pDeadEnda - (_DWORD)pMemory) )
    return 0;
    v5 = pMemory + 16;
    this->m_nRecordsNum = theHead.m_nRecordsNum;
    this->m_nFieldsNum = theHead.m_nFieldsNum;
    this->m_nStringBufSize = theHead.m_nStringBlockSize;
    this->m_pStringBuf = (char *)operator new[](theHead.m_nStringBlockSize);
    if ( !this->m_pStringBuf )
    return 0;
    vFieldType.vector();
    vFieldType.resize(theHead.m_nFieldsNum, 0);
    v6 = 4LL * theHead.m_nFieldsNum;
    v7 = std::vector<unsigned int>::operator[](&vFieldType, 0);
    memcpy(v7, v5, v6);
    v8 = &v5[4 * theHead.m_nFieldsNum];
    this->m_theType.resize(theHead.m_nFieldsNum, CDBCFile::FIELD_TYPE::T_INT);
    for ( i = 0; ; ++i )
    {
    if ( theHead.m_nFieldsNum <= i )
    {
    CDBCFile::FIELD::FIELD(&__x);
    this->m_vDataBuf.resize(theHead.m_nRecordsNum * theHead.m_nFieldsNum, __x);
    v10 = 8 * theHead.m_nRecordsNum * (__int64)theHead.m_nFieldsNum;
    v11 = std::vector<CDBCFile::FIELD>::operator[](&this->m_vDataBuf, 0);
    memcpy(v11, v8, v10);
    memcpy(this->m_pStringBuf, &v8[8 * theHead.m_nRecordsNum * (__int64)theHead.m_nFieldsNum], this->m_nStringBufSize);
    this->m_pStringBuf[this->m_nStringBufSize - 1] = 0;
    for ( j = 0; theHead.m_nFieldsNum > j; ++j )
    {
    if ( *std::vector<unsigned int>::operator[](&vFieldType, j) == 2 )
    {
    for ( i_0 = 0; theHead.m_nRecordsNum > i_0; ++i_0 )
    {
    FieldsNum = this->GetFieldsNum();
    v13 = std::vector<CDBCFile::FIELD>::operator[](&this->m_vDataBuf, j + i_0 * FieldsNum);
    v13->pString += (unsigned __int64)this->m_pStringBuf;
    }
    }
    }
    this->CreateIndex(0, szFileName);
    v4 = 1;
    goto LABEL_22;
    }
    if ( *std::vector<unsigned int>::operator[](&vFieldType, i) > 2 )
    break;
    v9 = std::vector<CDBCFile::FIELD_TYPE>::operator[](&this->m_theType, i);
    *v9 = *std::vector<unsigned int>::operator[](&vFieldType, i);
    }
    if ( this->m_pStringBuf )
    operator delete[](this->m_pStringBuf);
    v4 = 0;
    LABEL_22:
    std::vector<unsigned int>::~vector(&vFieldType);
    return v4;
}

bool CDBCFile::OpenFromMemoryImpl_Text(const char *pMemory, const char *pDeadEnd, const char *szFileName)
{

    const char *LineFromMemory; // r12


    const char *v12; // r12


    int j; // ebx


    const char *v20; // rax

    const char *v22; // rax


    const char *v29; // rax

    std::pair<const std::string,int> *v31; // rax

    std::pair<std::string,int> *v33; // rax

    std::pair<std::string,int> *v35; // rax
    const void *v36; // rax
    std::pair<std::string,int> *v37; // rax


    int k; // ebx
    int m; // r12d
    char szLine[10240]; // [rsp+30h] [rbp-2980h] BYREF
    std::map<std::string,int> mapStringBuf; // [rsp+2830h] [rbp-180h] BYREF
    std::_Rb_tree_iterator<std::pair<const std::string,int> > it; // [rsp+2860h] [rbp-150h] BYREF
    CDBCFile::FIELD newField; // [rsp+2870h] [rbp-140h] BYREF
    std::vector<std::pair<std::string,int>> vStringBuf; // [rsp+2880h] [rbp-130h] BYREF
    CDBCFile::FILEDS_TYPE vFieldsType; // [rsp+28A0h] [rbp-110h] BYREF
    std::vector<std::string> vRet; // [rsp+28C0h] [rbp-F0h] BYREF
    std::string __x; // [rsp+28E0h] [rbp-D0h] BYREF

    std::_Rb_tree_iterator<std::pair<const std::string,int> > v54; // [rsp+28F0h] [rbp-C0h] BYREF
    std::pair<std::string,int> p___x; // [rsp+2900h] [rbp-B0h] BYREF
    std::string __y[2]; // [rsp+2910h] [rbp-A0h] BYREF
    std::pair<std::_Rb_tree_iterator<std::pair<const std::string,int> >,bool> v57; // [rsp+2920h] [rbp-90h]
    std::pair<const std::string,int> v58; // [rsp+2930h] [rbp-80h] BYREF
    std::pair<std::string,int> v59; // [rsp+2940h] [rbp-70h] BYREF

    int nRecordsNum; // [rsp+295Ch] [rbp-54h]
    int nFieldsNum; // [rsp+2960h] [rbp-50h]
    int nStringBufSize; // [rsp+2964h] [rbp-4Ch]
    unsigned __int8 byBlank; // [rsp+296Bh] [rbp-45h]
    int i; // [rsp+296Ch] [rbp-44h]
    int nSubNum; // [rsp+2970h] [rbp-40h]
    int i_0; // [rsp+2974h] [rbp-3Ch]

    int i_1; // [rsp+2984h] [rbp-2Ch]
    CDBCFile::FIELD *theField; // [rsp+2988h] [rbp-28h]

    memset(szLine, 0, sizeof(szLine));
    LineFromMemory = CDBCFile::_GetLineFromMemory(szLine, 10240, pMemory, pDeadEnd);
    if ( !LineFromMemory )
    return 0;
    vRet.vector();
    CDBCFile::_ConvertStringToVector(szLine, &vRet, "\t", 1, 1);
    if ( vRet.empty() )
    {
    v6 = 0;
    }
    else
    {
    vFieldsType.vector();
    v7 = vRet.size();
    vFieldsType.resize(v7, CDBCFile::FIELD_TYPE::T_INT);
    for ( i = 0; ; ++i )
    {
    v11 = vRet.size();
    if ( v11 <= i )
    break;
    v8 = std::vector<std::string>::operator[](&vRet, i);
    if ( std::operator==<char>(v8, "INT") )
    {
    *std::vector<CDBCFile::FIELD_TYPE>::operator[](&vFieldsType, i) = CDBCFile::FIELD_TYPE::T_INT;
    }
    else
    {
    v9 = std::vector<std::string>::operator[](&vRet, i);
    if ( std::operator==<char>(v9, "FLOAT") )
    {
    *std::vector<CDBCFile::FIELD_TYPE>::operator[](&vFieldsType, i) = CDBCFile::FIELD_TYPE::T_FLOAT;
    }
    else
    {
    v10 = std::vector<std::string>::operator[](&vRet, i);
    if ( !std::operator==<char>(v10, "STRING") )
    {
    v6 = 0;
    goto LABEL_57;
    }
    *std::vector<CDBCFile::FIELD_TYPE>::operator[](&vFieldsType, i) = CDBCFile::FIELD_TYPE::T_STRING;
    }
    }
    }
    nRecordsNum = 0;
    nFieldsNum = vFieldsType.size();
    vStringBuf.vector();
    mapStringBuf.map();
    v12 = CDBCFile::_GetLineFromMemory(szLine, 10240, LineFromMemory, pDeadEnd);
    if ( v12 )
    {
    nStringBufSize = 0;
    while ( 1 )
    {
    v12 = CDBCFile::_GetLineFromMemory(szLine, 10240, v12, pDeadEnd);
    if ( !v12 )
    break;
    if ( szLine[0] != 35 )
    {
    CDBCFile::_ConvertStringToVector(szLine, &vRet, "\t", 1, 0);
    if ( !vRet.empty() )
    {
    v13 = vRet.size();
    if ( v13 != nFieldsNum )
    {
    v14 = vRet.size();
    if ( v14 < nFieldsNum )
    {
    v15 = vRet.size();
    nSubNum = nFieldsNum - v15;
    for ( i_0 = 0; i_0 < nSubNum; ++i_0 )
    {

    std::string::string(&__x, &unk_8F6A53);
    vRet.push_back(&__x);
    std::string::~string(&__x);
    std::allocator<char>::~allocator(&v53);
    }
    }
    }
    v16 = std::vector<std::string>::operator[](&vRet, 0);
    if ( !(unsigned __int8)std::string::empty(v16) )
    {
    for ( j = 0; j < nFieldsNum; ++j )
    {
    CDBCFile::FIELD::FIELD(&newField);
    v18 = *std::vector<CDBCFile::FIELD_TYPE>::operator[](&vFieldsType, j);
    switch ( v18 )
    {
    case CDBCFile::FIELD_TYPE::T_FLOAT:
    v21 = std::vector<std::string>::operator[](&vRet, j);
    v22 = (const char *)std::string::c_str(v21);
    v4.m128d_f64[0] = atof(v22);
    v4 = (__m128d)_mm_cvtpd_ps(_mm_unpacklo_pd(v4, v4));
    newField.iValue = LODWORD(v4.m128d_f64[0]);
    this->m_vDataBuf.push_back(&newField);
    break;
    case CDBCFile::FIELD_TYPE::T_STRING:
    v23 = std::vector<std::string>::operator[](&vRet, j);
    if ( (unsigned __int8)std::string::empty(v23) )
    {
    newField.iValue = 0;
    }
    else
    {
    v24 = std::vector<std::string>::operator[](&vRet, j);
    v68 = std::string::c_str(v24);
    v25 = std::vector<std::string>::operator[](&vRet, j);
    it = mapStringBuf.find(v25);
    v54 = mapStringBuf.end();
    if ( std::_Rb_tree_iterator<std::pair<std::string const,int>>::operator==(&it, &v54) )
    {
    v26 = std::vector<std::string>::operator[](&vRet, j);
    std::string::string(__y, v26);
    std::make_pair<std::string,int>(&p___x.first, (int)__y);
    vStringBuf.push_back(&p___x);
    std::pair<std::string,int>::~pair(&p___x);
    std::string::~string(__y);
    vStringBuf.size();
    v27 = std::vector<std::string>::operator[](&vRet, j);
    std::string::string(&v60, v27);
    std::make_pair<std::string,int>(&v59.first, (int)&v60);
    std::pair<std::string const,int>::pair<std::string,int>(&v58, &v59);
    v57 = mapStringBuf.insert(&v58);
    std::pair<std::string const,int>::~pair(&v58);
    std::pair<std::string,int>::~pair(&v59);
    std::string::~string(&v60);
    newField.iValue = nStringBufSize + 1;
    v28 = std::vector<std::string>::operator[](&vRet, j);
    v29 = (const char *)std::string::c_str(v28);
    v30 = strlen(v29);
    nStringBufSize += v30 + 1;
    }
    else
    {
    v31 = std::_Rb_tree_iterator<std::pair<std::string const,int>>::operator->(&it);
    newField.iValue = std::vector<std::pair<std::string,int>>::operator[](&vStringBuf, v31->second)->second
    + 1;
    }
    }
    this->m_vDataBuf.push_back(&newField);
    break;
    case CDBCFile::FIELD_TYPE::T_INT:
    v19 = std::vector<std::string>::operator[](&vRet, j);
    v20 = (const char *)std::string::c_str(v19);
    newField.iValue = atoi(v20);
    this->m_vDataBuf.push_back(&newField);
    break;
    }
    }
    ++nRecordsNum;
    }
    }
    }
    }
    this->m_nRecordsNum = nRecordsNum;
    this->m_nFieldsNum = nFieldsNum;
    this->m_nStringBufSize = nStringBufSize + 1;
    this->m_pStringBuf = (char *)operator new[](this->m_nStringBufSize);
    if ( this->m_pStringBuf )
    {
    this->m_theType = vFieldsType;
    byBlank = 0;
    *this->m_pStringBuf = 0;
    v32 = this->m_pStringBuf + 1;
    for ( i_1 = 0; ; ++i_1 )
    {
    v39 = vStringBuf.size();
    if ( v39 <= i_1 )
    break;
    v33 = std::vector<std::pair<std::string,int>>::operator[](&vStringBuf, i_1);
    v34 = std::string::size(&v33->first);
    v35 = std::vector<std::pair<std::string,int>>::operator[](&vStringBuf, i_1);
    v36 = (const void *)std::string::c_str(&v35->first);
    memcpy(v32, v36, v34);
    v37 = std::vector<std::pair<std::string,int>>::operator[](&vStringBuf, i_1);
    v38 = &v32[std::string::size(&v37->first)];
    *v38 = 0;
    v32 = v38 + 1;
    }
    for ( k = 0; k < nFieldsNum; ++k )
    {
    if ( *std::vector<CDBCFile::FIELD_TYPE>::operator[](&vFieldsType, k) == CDBCFile::FIELD_TYPE::T_STRING )
    {
    for ( m = 0; m < nRecordsNum; ++m )
    {
    theField = std::vector<CDBCFile::FIELD>::operator[](&this->m_vDataBuf, k + nFieldsNum * m);
    theField->pString = &this->m_pStringBuf[theField->iValue];
    }
    }
    }
    this->CreateIndex(0, szFileName);
    v6 = 1;
    }
    else
    {
    v6 = 0;
    }
    }
    else
    {
    v6 = 0;
    }
    /* mapStringBuf.~map(); */
    std::vector<std::pair<std::string,int>>::~vector(&vStringBuf);
    LABEL_57:
    std::vector<CDBCFile::FIELD_TYPE>::~vector(&vFieldsType);
    }
    std::vector<std::string>::~vector(&vRet);
    return v6;
}

bool CDBCFile::OpenFromTXT(const char *szFileName)
{
    int nFileSize; // [rsp+1Ch] [rbp-24h]
    char *pMemory; // [rsp+20h] [rbp-20h]
    bool bRet; // [rsp+2Fh] [rbp-11h]

    if ( !szFileName )
    __assert_fail("szFileName", "DBCFile.cpp", 0x1B0u, "bool CDBCFile::OpenFromTXT(const char*)");
    this->m_pf = fopen(szFileName, "rb");
    if ( !this->m_pf )
    return 0;
    fseek(this->m_pf, 0, 2);
    nFileSize = ftell(this->m_pf);
    fseek(this->m_pf, 0, 0);
    pMemory = (char *)operator new[](nFileSize + 1);
    fread(pMemory, 1u, nFileSize, this->m_pf);
    pMemory[nFileSize] = 0;
    bRet = this->OpenFromMemory(pMemory, &pMemory[nFileSize + 1], szFileName);
    if ( pMemory )
    operator delete[](pMemory);
    return bRet;
}

void CDBCFile::CreateIndex(int nColum, const char *szFileName)
{
    __gnu_cxx::_Hashtable_iterator<std::pair<const int,CDBCFile::FIELD*>,int,__gnu_cxx::hash<int>,std::_Select1st<std::pair<const int,CDBCFile::FIELD*> >,std::equal_to<int>,std::allocator<CDBCFile::FIELD*> > v3; // rax

    char szTemp[272]; // [rsp+30h] [rbp-190h] BYREF
    __gnu_cxx::_Hashtable_iterator<std::pair<const int,CDBCFile::FIELD*>,int,__gnu_cxx::hash<int>,std::_Select1st<std::pair<const int,CDBCFile::FIELD*> >,std::equal_to<int>,std::allocator<CDBCFile::FIELD*> > itFind; // [rsp+140h] [rbp-80h] BYREF
    __gnu_cxx::_Hashtable_iterator<std::pair<const int,CDBCFile::FIELD*>,int,__gnu_cxx::hash<int>,std::_Select1st<std::pair<const int,CDBCFile::FIELD*> >,std::equal_to<int>,std::allocator<CDBCFile::FIELD*> > __it; // [rsp+150h] [rbp-70h] BYREF
    std::pair<__gnu_cxx::_Hashtable_iterator<std::pair<const int,CDBCFile::FIELD*>,int,__gnu_cxx::hash<int>,std::_Select1st<std::pair<const int,CDBCFile::FIELD*> >,std::equal_to<int>,std::allocator<CDBCFile::FIELD*> >,bool> v9; // [rsp+160h] [rbp-60h] BYREF
    std::pair<const int,CDBCFile::FIELD*> __obj; // [rsp+180h] [rbp-40h] BYREF
    std::pair<int,CDBCFile::FIELD*> __p; // [rsp+190h] [rbp-30h] BYREF
    int i; // [rsp+1A4h] [rbp-1Ch]
    CDBCFile::FIELD *p; // [rsp+1A8h] [rbp-18h]
    std::pair<int,CDBCFile::FIELD*> v14; // 0:kr00_16.16

    if ( nColum >= 0 && this->m_nFieldsNum > nColum && this->m_nIndexColum != nColum )
    {
    __gnu_cxx::hash_map<int,CDBCFile::FIELD *,__gnu_cxx::hash<int>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::clear(&this->m_hashIndex);
    for ( i = 0; this->m_nRecordsNum > i; ++i )
    {
    p = std::vector<CDBCFile::FIELD>::operator[](&this->m_vDataBuf, i * this->m_nFieldsNum);
    itFind = __gnu_cxx::hash_map<int,CDBCFile::FIELD *,__gnu_cxx::hash<int>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::find(
    &this->m_hashIndex,
    (const int *const)p);
    v3 = __gnu_cxx::hash_map<int,CDBCFile::FIELD *,__gnu_cxx::hash<int>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::end(&this->m_hashIndex);
    HIDWORD(v4) = HIDWORD(v3._M_cur);
    __it = v3;
    if ( __gnu_cxx::_Hashtable_iterator<std::pair<int const,CDBCFile::FIELD *>,int,__gnu_cxx::hash<int>,std::_Select1st<std::pair<int const,CDBCFile::FIELD *>>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::operator!=(
    &itFind,
    &__it) )
    {
    snprintf(szTemp, 0x104u, "[%s]Multi index at line %d(SAME:value=%d)", szFileName, i + 1, p->iValue);
    return;
    }
    v14 = std::make_pair<int,CDBCFile::FIELD *>(p->iValue, p);
    LODWORD(v4) = v14.first;
    *(_QWORD *)&__p.first = v4;
    __p.second = v14.second;
    std::pair<int const,CDBCFile::FIELD *>::pair<int,CDBCFile::FIELD *>(&__obj, &__p);
    __gnu_cxx::hash_map<int,CDBCFile::FIELD *,__gnu_cxx::hash<int>,std::equal_to<int>,std::allocator<CDBCFile::FIELD *>>::insert(
    &v9,
    &this->m_hashIndex,
    &__obj);
    }
    }
}

（内容由AI生成，仅供参考）
