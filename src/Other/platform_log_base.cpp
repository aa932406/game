#include "Other/platform_log_base.h"

platform_log_base::platform_log_base()
{
    CleanUp();
}

platform_log_base::~platform_log_base()
{
}

void platform_log_base::CleanUp()
{
    log_time = 0;
    log_Sid = 0;
}

void platform_log_base::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(log_time);
    packet->writeInt32(log_Sid);
}

void platform_log_base::UnPackageData(Answer::NetPacket *inPacket)
{
    log_time = inPacket->readInt32();
    log_Sid = inPacket->readInt32();
}
