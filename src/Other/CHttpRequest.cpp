#include "Other/CHttpRequest.h"

#include <cstring>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Utility/Answer.h"

CHttpRequest::CHttpRequest()
    : m_bRunning(false)
{
}

CHttpRequest::~CHttpRequest()
{
}

void CHttpRequest::run()
{
    m_bRunning = true;
    while (m_bRunning)
    {
        SendGetRequest();
        Answer::DayTime::sleep(100);
    }
}

void CHttpRequest::Init(std::string *p_ip_0, std::string *p_path)
{
    m_ServerIp = *p_ip_0;
    m_ServerPath = *p_path;
}

void CHttpRequest::Stop()
{
    m_bRunning = false;
    join();
}

void CHttpRequest::SendGetRequest()
{
    Answer::MutexGuard lock(&m_GetStringLock);
    while (!m_lGetReqString.empty())
    {
        std::string ReqString = m_lGetReqString.front();
        m_lGetReqString.pop_front();
        std::string p_param = ReqString;
        std::string p_path = m_ServerPath;
        std::string p_ip_0 = m_ServerIp;
        sendGetRequest(&p_ip_0, 80, &p_path, &p_param);
    }
}

std::string CHttpRequest::UrlEncode(const std::string *const str)
{
    std::string result;
    size_t length = str->length();
    for (size_t i = 0; i < length; ++i)
    {
        unsigned char c = (*str)[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '=' || c == '&')
        {
            result += (char)c;
        }
        else if (c == ' ')
        {
            result += "+";
        }
        else
        {
            result += '%';
            result += (char)ToHex(c >> 4);
            result += (char)ToHex(c & 0xF);
        }
    }
    return result;
}

unsigned __int8 CHttpRequest::ToHex(unsigned __int8 x)
{
    if (x <= 9)
        return x + 48;
    else
        return x + 55;
}

bool CHttpRequest::isValidIpAddress(const char *ipAddress)
{
    return inet_addr(ipAddress) != INADDR_NONE;
}

bool CHttpRequest::GetIpByDomainName(const char *szHost, char *szIp)
{
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *cur;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int ret = getaddrinfo(szHost, nullptr, &hints, &res);
    if (ret != 0)
        return false;

    for (cur = res; cur != nullptr; cur = cur->ai_next)
    {
        struct sockaddr_in *addr = (struct sockaddr_in *)cur->ai_addr;
        strcpy(szIp, inet_ntoa(addr->sin_addr));
        freeaddrinfo(res);
        return true;
    }
    freeaddrinfo(res);
    return false;
}
