#include "Other/MemChrDepotData.h"
#include "Other/Answer.h"

MemChrDepotData::MemChrDepotData()
{
    CleanUp();
}

MemChrDepotData::~MemChrDepotData()
{
}

void MemChrDepotData::CleanUp()
{
    memset(gambleDepot, 0, sizeof(gambleDepot));
    memset(Currency, 0, sizeof(Currency));
    OpneCount = 0;
    SecondPassword.clear();
    Password.clear();
}

void MemChrDepotData::PackageData(Answer::NetPacket *packet)
{
    uint32_t nCount = packet->getWOffset();
    int32_t gambleSlotCount = 0;
    packet->writeInt32(0);
    for (int32_t i = 0; i <= 559; ++i)
    {
        if (gambleDepot[i].itemCount > 0)
        {
            packet->writeInt32(i);
            packet->writeInt32(gambleDepot[i].itemId);
            packet->writeInt8(gambleDepot[i].itemClass);
            packet->writeInt32(gambleDepot[i].itemCount);
            packet->writeInt8(gambleDepot[i].bind);
            packet->writeInt32(gambleDepot[i].endTime);
            packet->writeInt64(gambleDepot[i].srcId);
            ++gambleSlotCount;
        }
    }
    *(int32_t*)&packet->getBuffer()[nCount] = gambleSlotCount;
    std::string CurrencyString = GetCurrencyString();
    packet->writeUTF8(CurrencyString);
    packet->writeInt32(OpneCount);
    packet->writeUTF8(Password);
    packet->writeUTF8(SecondPassword);
}

void MemChrDepotData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    int32_t slotCount = inPacket->readInt32();
    for (int32_t i = 0; i < slotCount; ++i)
    {
        int32_t slot = inPacket->readInt32();
        if (slot >= 560)
        {
            inPacket->readInt32();
            inPacket->readInt8();
            inPacket->readInt32();
            inPacket->readInt8();
            inPacket->readInt32();
            inPacket->readInt64();
            continue;
        }
        gambleDepot[slot].itemId = inPacket->readInt32();
        gambleDepot[slot].itemClass = inPacket->readInt8();
        gambleDepot[slot].itemCount = inPacket->readInt32();
        gambleDepot[slot].bind = inPacket->readInt8();
        gambleDepot[slot].endTime = inPacket->readInt32();
        gambleDepot[slot].srcId = inPacket->readInt64();
    }
    std::string Currency;
    inPacket->readUTF8(Currency);
    InitCurrency(&Currency);
    OpneCount = inPacket->readInt32();
    inPacket->readUTF8(Password);
    inPacket->readUTF8(SecondPassword);
}

std::string MemChrDepotData::GetCurrencyString()
{
    std::stringstream ss;
    for (int32_t i = 0; i < 12; ++i)
    {
        if (Currency[i] > 0)
        {
            ss << "|" << i << ":" << Currency[i];
        }
    }
    return ss.str();
}

void MemChrDepotData::InitCurrency(std::string *p_CurrencyString)
{
    StringVector vStr;
    Answer::StringUtility::split(&vStr, p_CurrencyString, "|", 0);
    int32_t isize = vStr.size();
    for (int32_t i = 0; i < isize; ++i)
    {
        StringVector tv;
        Answer::StringUtility::split(&tv, &vStr[i], ":", 0);
        if (tv.size() == 2)
        {
            int32_t nType = atoi(tv[0].c_str());
            if (nType >= 0 && nType < 12)
            {
                Currency[nType] = strtoll(tv[1].c_str(), nullptr, 10);
            }
        }
    }
}
