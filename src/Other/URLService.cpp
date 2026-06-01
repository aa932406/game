#include "Other/URLService.h"
#include "Network/NetPacket.h"
#include <string>
#include <cstring>

namespace Answer
{
    class TcpService
    {
    public:
        TcpService(int32_t sendBufSize, int32_t recvBufSize, uint32_t a3, int32_t a4, int32_t a5, const std::string& name) {}
        virtual ~TcpService() {}
    };

    class InetAddress
    {
    public:
        InetAddress() {}
        InetAddress(const std::string* host, uint16_t port) { (void)host; (void)port; }
        ~InetAddress() {}
    };

    template<typename T>
    class Singleton
    {
    public:
        static T* instance();
    };
}

URLService::URLService()
{
}

URLService::~URLService()
{
}

void URLService::onNetPacket(Answer::NetPacket* inPacket)
{
    if (inPacket)
        inPacket->destroy();
}

void URLService::getUrl(std::string* p_url)
{
    if (!p_url)
        return;

    std::string str_http = "http://";
    std::string quest = "GET ";
    quest += *p_url;
    quest += " HTTP/1.0\r\n";
    quest += "User-agent:Mozilla/4.0\r\n";
    quest += "Accept-language:zh-cn\r\n\r\n";

    size_t pos1 = p_url->find(str_http, 0);
    if (pos1 == std::string::npos)
        return;

    size_t pos2 = p_url->find("/", pos1 + 7);
    if (pos2 == std::string::npos)
        return;

    std::string hostname = p_url->substr(pos1 + 7, pos2 - pos1 - 7);
    (void)hostname;
    std::string filename = p_url->substr(p_url->find_last_of("/") + 1);
    (void)filename;
}
