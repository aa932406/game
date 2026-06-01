#ifndef _URLSERVICE_H_
#define _URLSERVICE_H_

#include <cstdint>
#include <string>

namespace Answer
{
    class NetPacket;
    class InetAddress;
    class TcpService;
    class TcpClient;
}

class URLService
{
public:
    URLService();
    virtual ~URLService();

    void onNetPacket(Answer::NetPacket* inPacket);
    void getUrl(std::string* p_url);
};

#endif // _URLSERVICE_H_
