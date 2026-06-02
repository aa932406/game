#include "Database/MailDBData.h"
#include "Answer/DayTime.h"

MailDBData::MailDBData() {
}

MailDBData::~MailDBData() {
}

void MailDBData::SaveToSqlString(SqlStringList* const sqls, char (*const szSQL)[4096], CharId_t nCid) {
    int32_t CurTime = Answer::DayTime::now();
    for (auto& pair : m_MailInfo) {
        auto& mail = pair.second;
        bzero(szSQL, 0x1000u);
        if (mail.SendTime + 1209600 >= CurTime) {
            snprintf((char*)szSQL, 0xFFFu,
                "UPDATE `mem_mail` SET `HasRead`=%d,`Extract`=%d WHERE `MailId`=%d",
                mail.HasRead, mail.Extract, mail.MailId);
        } else {
            snprintf((char*)szSQL, 0xFFFu,
                "UPDATE `mem_mail` SET `HasRead`=%d,`Extract`=%d, `DelFlag`=2,`DelTime`=%d WHERE `MailId`=%d",
                mail.HasRead, mail.Extract, CurTime, mail.MailId);
        }
        sqls->push_back(std::string((char*)szSQL));
    }
}

bool MailDBData::LoadFromDB(Answer::MySqlDBGuard* const db, char (*const szSQL)[4096], int64_t nUid, int32_t nSid, CharId_t nCid) {
    // TODO: 实现从数据库加载邮件
    (void)db;
    (void)szSQL;
    (void)nUid;
    (void)nSid;
    (void)nCid;
    return false;
}

void MailDBData::PackageData(Answer::NetPacket* packet) {
    if (!packet) return;
    int32_t nSize = static_cast<int32_t>(m_MailInfo.size());
    Answer::NetPacket::writeInt32(packet, nSize);
    for (auto& pair : m_MailInfo) {
        (void)pair;
        auto& mail = pair.second;
        Answer::NetPacket::writeInt32(packet, mail.MailId);
        Answer::NetPacket::writeInt32(packet, mail.SysMailId);
        Answer::NetPacket::writeInt64(packet, mail.SenderId);
        Answer::NetPacket::writeUTF8(packet, &mail.SenderName);
        Answer::NetPacket::writeInt64(packet, mail.ReceiveId);
        Answer::NetPacket::writeUTF8(packet, &mail.ReceiveName);
        Answer::NetPacket::writeInt32(packet, mail.SendTime);
        Answer::NetPacket::writeInt8(packet, mail.HasRead);
        Answer::NetPacket::writeInt8(packet, mail.Extract);
        Answer::NetPacket::writeUTF8(packet, &mail.MailTitle);
        Answer::NetPacket::writeUTF8(packet, &mail.MailContent);
        Answer::NetPacket::writeInt32(packet, mail.nReason);
        Answer::NetPacket::writeUTF8(packet, &mail.Param);
        if (mail.Extract == 1 || mail.Extract == 2) {
            int8_t Count = 0;
            int32_t Offset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::writeInt8(packet, 0);
            for (int8_t i = 0; i <= 5; ++i) {
                if (mail.Item[i].itemId > 0 && mail.Item[i].itemCount > 0) {
                    Answer::NetPacket::writeInt8(packet, i);
                    Answer::NetPacket::writeInt32(packet, mail.Item[i].itemId);
                    Answer::NetPacket::writeInt8(packet, mail.Item[i].itemClass);
                    Answer::NetPacket::writeInt32(packet, mail.Item[i].itemCount);
                    Answer::NetPacket::writeInt8(packet, mail.Item[i].bind);
                    Answer::NetPacket::writeInt32(packet, mail.Item[i].endTime);
                    Answer::NetPacket::writeInt64(packet, mail.Item[i].srcId);
                    ++Count;
                }
            }
            char* Buffer = Answer::NetPacket::getBuffer(packet);
            if (Buffer) Buffer[Offset] = Count;
        }
    }
}

void MailDBData::UnPackageData(Answer::NetPacket* inPacket, CharId_t nCid) {
    if (!inPacket) return;
    (void)nCid;
    int32_t nSize = Answer::NetPacket::readInt32(inPacket);
    for (int32_t i = 0; i < nSize; ++i) {
        MailInfo mail;
        mail.MailId = Answer::NetPacket::readInt32(inPacket);
        mail.SysMailId = Answer::NetPacket::readInt32(inPacket);
        mail.SenderId = Answer::NetPacket::readInt64(inPacket);
        Answer::NetPacket::readUTF8(inPacket, &mail.SenderName);
        mail.ReceiveId = Answer::NetPacket::readInt64(inPacket);
        Answer::NetPacket::readUTF8(inPacket, &mail.ReceiveName);
        mail.SendTime = Answer::NetPacket::readInt32(inPacket);
        mail.HasRead = Answer::NetPacket::readInt8(inPacket);
        mail.Extract = Answer::NetPacket::readInt8(inPacket);
        Answer::NetPacket::readUTF8(inPacket, &mail.MailTitle);
        Answer::NetPacket::readUTF8(inPacket, &mail.MailContent);
        mail.nReason = Answer::NetPacket::readInt32(inPacket);
        Answer::NetPacket::readUTF8(inPacket, &mail.Param);
        if (mail.Extract == 1 || mail.Extract == 2) {
            int8_t Count = Answer::NetPacket::readInt8(inPacket);
            if (Count > 6) return;
            for (int8_t j = 0; j < Count; ++j) {
                int8_t idx = Answer::NetPacket::readInt8(inPacket);
                mail.Item[idx].itemId = Answer::NetPacket::readInt32(inPacket);
                mail.Item[idx].itemClass = Answer::NetPacket::readInt8(inPacket);
                mail.Item[idx].itemCount = Answer::NetPacket::readInt32(inPacket);
                mail.Item[idx].bind = Answer::NetPacket::readInt8(inPacket);
                mail.Item[idx].endTime = Answer::NetPacket::readInt32(inPacket);
                mail.Item[idx].srcId = Answer::NetPacket::readInt64(inPacket);
            }
        }
        m_MailInfo[mail.MailId] = mail;
    }
}
