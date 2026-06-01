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
    void setType(PackType type);
    void setProc(uint16_t proc);
    void destroy();
};

} // namespace Answer

#endif // _NETPACKET_H_
