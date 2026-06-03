#ifndef _NETPACKET_H_
#define _NETPACKET_H_

#include <cstdint>
#include <string>

namespace Answer {

enum class PackType : uint8_t
{
    PACK_DISPATCH = 0,
    PACK_PROC = 1
};

class NetPacket
{
public:
    int32_t m_size;
    uint32_t m_wOffset;
    uint16_t m_proc;
    int32_t m_refCount;

    void writeInt32(int32_t value);
    void writeInt8(int8_t value);
    void writeInt64(int64_t value);
    void writeInt16(int16_t value);
    void writeUTF8(const std::string& value);

    int32_t readInt32();
    int8_t readInt8();
    int64_t readInt64();
    int16_t readInt16();
    void readUTF8(std::string& out);

    void setSize(uint32_t size);
    uint32_t getWOffset();
    void setWOffset(uint32_t offset);
    uint16_t getProc();
    PackType getType() { return PackType::PACK_PROC; }
    void setType(PackType type);
    void setProc(uint16_t proc);
    void destroy();
    bool rightShift(uint32_t n) { (void)n; return true; }
    uint32_t getSize() { return 0; }
    char* getBuffer() { return nullptr; }
    void write(const char* buf, uint32_t len) { (void)buf; (void)len; }

    // 静态包装方法
    static void writeInt32(NetPacket* p, int32_t v) { if (p) p->writeInt32(v); }
    static void writeInt8(NetPacket* p, int8_t v) { if (p) p->writeInt8(v); }
    static void writeInt64(NetPacket* p, int64_t v) { if (p) p->writeInt64(v); }
    static void writeInt16(NetPacket* p, int16_t v) { if (p) p->writeInt16(v); }
    static void writeUTF8(NetPacket* p, const std::string& v) { if (p) p->writeUTF8(v); }
    static void writeUTF8(NetPacket* p, const std::string* v) { if (p && v) p->writeUTF8(*v); }
    static int32_t readInt32(NetPacket* p) { return p ? p->readInt32() : 0; }
    static int8_t readInt8(NetPacket* p) { return p ? p->readInt8() : 0; }
    static int64_t readInt64(NetPacket* p) { return p ? p->readInt64() : 0; }
    static int16_t readInt16(NetPacket* p) { return p ? p->readInt16() : 0; }
    static void readUTF8(NetPacket* p, std::string& out) { if (p) p->readUTF8(out); }
    static void readUTF8(NetPacket* p, std::string* out) { if (p && out) p->readUTF8(*out); }
    static void setSize(NetPacket* p, uint32_t s) { if (p) p->setSize(s); }
    static uint32_t getWOffset(NetPacket* p) { return p ? p->getWOffset() : 0; }
    static void setWOffset(NetPacket* p, uint32_t o) { if (p) p->setWOffset(o); }
    static uint16_t getProc(NetPacket* p) { return p ? p->getProc() : 0; }
    static PackType getType(NetPacket* p) { return p ? p->getType() : PackType::PACK_PROC; }
    static void setType(NetPacket* p, PackType t) { if (p) p->setType(t); }
    static void setProc(NetPacket* p, uint16_t pr) { if (p) p->setProc(pr); }
    static void destroy(NetPacket* p) { if (p) p->destroy(); }
    static uint32_t getSize(NetPacket* p) { (void)p; return 0; }
    static char* getBuffer(NetPacket* p) { (void)p; return nullptr; }
    static void write(NetPacket* p, const char* buf, uint32_t len) { (void)p; (void)buf; (void)len; }
    static bool rightShift(NetPacket* p, uint32_t n) { return p ? p->rightShift(n) : false; }

    static NetPacket* allocPacket(int32_t a, uint16_t b, int32_t c, int32_t d) { (void)a; (void)b; (void)c; (void)d; return new NetPacket(); }
    static void writeString(NetPacket* p, const std::string* v) { if (p && v) { p->writeUTF8(*v); } }
    static std::string readString(NetPacket* p) { std::string out; if (p) p->readUTF8(out); return out; }
};

} // namespace Answer

#endif // _NETPACKET_H_
