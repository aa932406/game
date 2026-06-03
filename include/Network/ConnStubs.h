#ifndef _CONNSTUBS_H_
#define _CONNSTUBS_H_

#include <cstdint>
#include <string>

namespace Answer {

class NetPacket;
class InetAddress;

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
    Answer::NetPacket* popNetpacket() { return nullptr; }
    Answer::NetPacket* popNetpacket(uint32_t size) { (void)size; return nullptr; }

    static void sendPacket(TcpClient* p, Answer::NetPacket* packet) { (void)p; (void)packet; }
    void sendPacket(Answer::NetPacket* packet) { (void)packet; }

    static void getName(TcpClient* p, std::string* out) { if (out) *out = ""; }
    static void getName2(TcpClient* p, std::string& out) { (void)p; out = ""; }
};

class Thread {
public:
    Thread() {}
    ~Thread() {}
    static void start(void* pConn) { (void)pConn; }
};

class TcpService {
public:
    static void sendPacketTo(void* pConn, int16_t cgindex, Answer::NetPacket* packet) { (void)pConn; (void)cgindex; (void)packet; }
    static int32_t replySuccess(void* pConn, int16_t cgindex, uint16_t proc, int64_t addon) { (void)pConn; (void)cgindex; (void)proc; (void)addon; return 0; }
    static int32_t replyfailure(void* pConn, int16_t cgindex, uint16_t proc, int32_t errcode, int64_t addon) { (void)pConn; (void)cgindex; (void)proc; (void)errcode; (void)addon; return 10002; }
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
    int8_t GetId() const { return m_id; }

    void connect(Answer::InetAddress* addr) { (void)addr; }
    void start() {}
    Answer::NetPacket* popNetpacket() { return nullptr; }
    Answer::NetPacket* popNetpacket(uint32_t size) { (void)size; return nullptr; }
    void sendPacket(Answer::NetPacket* packet) { (void)packet; }
    void sendPacketTo(int16_t cgindex, Answer::NetPacket* packet) { (void)cgindex; (void)packet; }
    int32_t replySuccess(int16_t cgindex, uint16_t proc, int64_t addon) { (void)cgindex; (void)proc; (void)addon; return 0; }
    int32_t replyfailure(int16_t cgindex, uint16_t proc, int32_t errcode, int64_t addon) { (void)cgindex; (void)proc; (void)errcode; (void)addon; return 10002; }

    int8_t m_id;
    T* m_svc;
};

#endif // _CONNSTUBS_H_
