#ifndef _CONNSTUBS_H_
#define _CONNSTUBS_H_

#include <cstdint>
#include <string>

namespace Answer {

class InetAddress {
public:
    InetAddress() {}
    InetAddress(std::string* host, int32_t port) { (void)host; (void)port; }
    ~InetAddress() {}
};

class TcpClient {
public:
    TcpClient() {}
    ~TcpClient() {}

    static bool connect(TcpClient* p, Answer::InetAddress* addr) { (void)p; (void)addr; return false; }
    bool connect(Answer::InetAddress* addr) { (void)addr; return false; }
    void start() {}
    static void start(TcpClient* p) { if (p) p->start(); }

    static Answer::NetPacket* popNetpacket(void* p) { (void)p; return nullptr; }

    template<typename T, typename U>
    static void sendPacket(T* p, U* packet) { (void)p; (void)packet; }

    template<typename T>
    static void getName(T* val) { (void)val; }
};

class Thread {
public:
    Thread() {}
    ~Thread() {}
    static void start(void* pConn) { (void)pConn; }
};

} // namespace Answer

template<typename T>
class MutiConn {
public:
    MutiConn() : m_id(0), m_svc(nullptr) {}
    MutiConn(size_t a, size_t b, size_t c, int d, int e, int f, const std::string& name, int8_t id, T* svc)
        : m_id(id), m_svc(svc) { (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)name; }
    ~MutiConn() {}

    static int8_t GetId(MutiConn* p) { return p ? p->m_id : 0; }

    int8_t m_id;
    T* m_svc;
};

#endif // _CONNSTUBS_H_
