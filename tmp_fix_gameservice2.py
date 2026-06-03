#!/usr/bin/env python3
"""Second pass: fix manual ctors/dtors, NetPacket static calls, vtable calls."""
import re

with open("src/Game/GameService.cpp", "r") as f:
    content = f.read()

# ===== 1. Fix Answer::NetPacket static calls =====
# Answer::NetPacket::readInt32(packet) -> packet->readInt32()
content = re.sub(r'Answer::NetPacket::readInt32\((\w+)\)', r'\1->readInt32()', content)
content = re.sub(r'Answer::NetPacket::readInt8\((\w+)\)', r'\1->readInt8()', content)
content = re.sub(r'Answer::NetPacket::readInt64\((\w+)\)', r'\1->readInt64()', content)
content = re.sub(r'Answer::NetPacket::readInt16\((\w+)\)', r'\1->readInt16()', content)
content = re.sub(r'Answer::NetPacket::readUTF8\((\w+),\s*(\w+)\)', r'\1->readUTF8(\2)', content)
content = re.sub(r'Answer::NetPacket::readUTF8\((\w+),\s*\(bool\)(\w+)\)', r'\1->readUTF8(\2)', content)
content = re.sub(r'Answer::NetPacket::writeInt32\((\w+),\s*(\w+)\)', r'\1->writeInt32(\2)', content)
content = re.sub(r'Answer::NetPacket::writeInt8\((\w+),\s*(\w+)\)', r'\1->writeInt8(\2)', content)
content = re.sub(r'Answer::NetPacket::writeInt64\((\w+),\s*(\w+)\)', r'\1->writeInt64(\2)', content)
content = re.sub(r'Answer::NetPacket::writeInt16\((\w+),\s*(\w+)\)', r'\1->writeInt16(\2)', content)
content = re.sub(r'Answer::NetPacket::writeUTF8\((\w+),\s*(\w+)\)', r'\1->writeUTF8(\2)', content)
content = re.sub(r'Answer::NetPacket::getWOffset\((\w+)\)', r'\1->getWOffset()', content)
content = re.sub(r'Answer::NetPacket::setSize\((\w+),\s*(\w+)\)', r'\1->setSize(\2)', content)
content = re.sub(r'Answer::NetPacket::setWOffset\((\w+),\s*(\w+)\)', r'\1->setWOffset(\2)', content)
content = re.sub(r'Answer::NetPacket::getProc\((\w+)\)', r'\1->getProc()', content)
content = re.sub(r'Answer::NetPacket::setType\((\w+),\s*(\w+)\)', r'\1->setType(\2)', content)
content = re.sub(r'Answer::NetPacket::setProc\((\w+),\s*(\w+)\)', r'\1->setProc(\2)', content)
content = re.sub(r'Answer::NetPacket::getSize\((\w+)\)', r'\1->getSize()', content)
content = re.sub(r'Answer::NetPacket::getBuffer\((\w+)\)', r'\1->getBuffer()', content)
content = re.sub(r'Answer::NetPacket::write\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->write(\2, \3)', content)
content = re.sub(r'Answer::NetPacket::destroy\((\w+)\)', r'\1->destroy()', content)
content = re.sub(r'Answer::NetPacket::rightShift\((\w+),\s*(\w+)u?\)', r'\1->rightShift(\2)', content)

# ===== 2. Fix manual constructor calls =====
# Answer::InetAddress::InetAddress(&addr, ...) -> Answer::InetAddress addr(...)
content = re.sub(
    r'Answer::InetAddress::InetAddress\(&(\w+),\s*(\w+),\s*(\w+)\);',
    r'Answer::InetAddress \1(\2, \3);',
    content
)
content = re.sub(
    r'Answer::InetAddress::~InetAddress\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)

# Answer::MutexGuard::MutexGuard(&lock, ...) -> Answer::MutexGuard lock(...)
content = re.sub(
    r'Answer::MutexGuard::MutexGuard\(&(\w+),\s*(.+?)\);',
    r'Answer::MutexGuard \1(\2);',
    content
)
content = re.sub(
    r'Answer::MutexGuard::~MutexGuard\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)

# Answer::Inifile::Inifile(&ini) -> Answer::Inifile ini;
content = re.sub(
    r'Answer::Inifile::Inifile\(&(\w+)\);',
    r'/* Answer::Inifile \1; (will be handled inline) */',
    content
)
content = re.sub(
    r'Answer::Inifile::parse\(&(\w+),\s*&(\w+)\);',
    r'\1.parse(\2);',
    content
)
content = re.sub(
    r'Answer::Inifile::getStrValue\(&(\w+),\s*&(\w+),\s*&(\w+)\);',
    r'\1.getStrValue(\2, \3);',
    content
)
content = re.sub(
    r'Answer::Inifile::~Inifile\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)
content = re.sub(
    r'Answer::Inifile::Inifile\(&(\w+)\);',
    r'Answer::Inifile \1;\n    \1 = Answer::Inifile();',
    content
)

# Answer::MD5::MD5(&md5) -> Answer::MD5 md5;
content = re.sub(
    r'Answer::MD5::MD5\(&(\w+)\);',
    r'Answer::MD5 \1;',
    content
)
content = re.sub(
    r'Answer::MD5::md5sum\(&(\w+),\s*&(\w+)\);',
    r'\1.md5sum(\2);',
    content
)
# Answer::MD5::~MD5(&md5) 
content = re.sub(
    r'Answer::MD5::~MD5\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)

# Answer::MySqlDBGuard::MySqlDBGuard(&db, ...) -> Answer::MySqlDBGuard db(...)
content = re.sub(
    r'Answer::MySqlDBGuard::MySqlDBGuard\(&(\w+),\s*(.+?)\);',
    r'Answer::MySqlDBGuard \1(\2);',
    content
)
content = re.sub(
    r'Answer::MySqlDBGuard::~MySqlDBGuard\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)
content = re.sub(
    r'Answer::MySqlDBGuard::excute\(&(\w+),\s*(\w+)\)',
    r'\1.excute(\2)',
    content
)
content = re.sub(
    r'Answer::MySqlDBGuard::query\(&(\w+),\s*(\w+)\)',
    r'\1.query(\2)',
    content
)

# Answer::MySqlQuery::MySqlQuery(&result, ...) -> Answer::MySqlQuery result(...)
content = re.sub(
    r'Answer::MySqlQuery::MySqlQuery\(&(\w+),\s*(.+?)\);',
    r'Answer::MySqlQuery \1(\2);',
    content
)
content = re.sub(
    r'Answer::MySqlQuery::~MySqlQuery\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)
content = re.sub(
    r'Answer::MySqlQuery::eof\(&(\w+)\)',
    r'\1.eof()',
    content
)
content = re.sub(
    r'Answer::MySqlQuery::getIntValue\(&(\w+),\s*\"(\w+)\",\s*(\w+)\)',
    r'\1.getIntValue("\2", \3)',
    content
)
content = re.sub(
    r'Answer::MySqlQuery::nextRow\(&(\w+)\)',
    r'\1.nextRow()',
    content
)

# TencentInfo::TencentInfo(&info) -> TencentInfo info;
content = re.sub(
    r'TencentInfo::TencentInfo\(&(\w+)\);',
    r'TencentInfo \1;',
    content
)
content = re.sub(
    r'TencentInfo::~TencentInfo\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)
content = re.sub(
    r'TencentInfo::UnPacketInfo\(&(\w+),\s*(\w+)\)',
    r'\1.UnPacketInfo(\2)',
    content
)

# PlayerDBData::PlayerDBData(&dbData) -> PlayerDBData dbData;
content = re.sub(
    r'PlayerDBData::PlayerDBData\(&(\w+)\);',
    r'PlayerDBData \1;',
    content
)
content = re.sub(
    r'PlayerDBData::~PlayerDBData\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)

# std::ofstream manual construction
content = re.sub(
    r'std::ofstream::basic_ofstream\((\w+),\s*(\w+),\s*(\w+)\);',
    r'std::ofstream \1(\2, \3);',
    content
)
content = re.sub(
    r'std::ofstream::~ofstream\((\w+)\);',
    r'/* \1 auto-destructed */',
    content
)

# ===== 3. Fix specific unanswered patterns =====
# Fix Answer::Logger static calls
content = re.sub(
    r'Answer::Logger::print\(Answer::LogLevel::(\w+),\s*"([^"]*)",\s*(\w+)\)',
    r'// LOG(\1, "\2", \3)',
    content
)
content = re.sub(
    r'Answer::Logger::print\(Answer::LogLevel::(\w+),\s*"([^"]*)",\s*(\w+),\s*(\w+)\)',
    r'// LOG(\1, "\2", \3, \4)',
    content
)
content = re.sub(
    r'Answer::Logger::print\(Answer::LogLevel::(\w+),\s*"([^"]*)",\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'// LOG(\1, "\2", \3, \4, \5)',
    content
)
content = re.sub(
    r'Answer::Logger::print\(Answer::LogLevel::(\w+),\s*"([^"]*)",\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'// LOG(\1, "\2", \3, \4, \5, \6)',
    content
)
content = re.sub(
    r'Answer::Logger::print\(Answer::LogLevel::(\w+),\s*"([^"]*)",\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'// LOG(\1, "\2", \3, \4, \5, \6, \7)',
    content
)
content = re.sub(
    r'Answer::Logger::print\(Answer::LogLevel::(\w+),\s*"([^"]*)",\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'// LOG(\1, "\2", \3, \4, \5, \6, \7, \8)',
    content
)

# Fix CfgData:: static calls
content = re.sub(r'CfgData::getDebug\((\w+)\)', r'\1->getDebug()', content)
content = re.sub(r'CfgData::getMapRegion\((\w+),\s*(\w+)\)', r'\1->getMapRegion(\2)', content)
content = re.sub(r'CfgData::reload\((\w+)\)', r'\1->reload()', content)
content = re.sub(r'CfgData::getDungeon\((\w+),\s*(\w+)\)', r'\1->getDungeon(\2)', content)
content = re.sub(r'CfgData::getServerDiffDay\((\w+),\s*(\w+)\)', r'\1->getServerDiffDay(\2)', content)

# Fix DBService:: static calls
content = re.sub(r'DBService::OnUpdatePay\((\w+)\)', r'\1->OnUpdatePay()', content)
content = re.sub(r'DBService::onNewMinuteCome\((\w+),\s*(\w+)\)', r'\1->onNewMinuteCome(\2)', content)
content = re.sub(r'DBService::loadPlayer\((\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)', r'\1->loadPlayer(\2, \3, \4, \5, \6)', content)
content = re.sub(r'DBService::UpdateLoginTime\((\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)', r'\1->UpdateLoginTime(\2, \3, \4)', content)
content = re.sub(r'DBService::SaveDropTimes\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->SaveDropTimes(\2, \3)', content)
content = re.sub(r'DBService::ClearDropTimes\((\w+)\)', r'\1->ClearDropTimes()', content)
content = re.sub(r'DBService::OnSendSysMail\((\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)', r'\1->OnSendSysMail(\2, \3, \4, \5, \6)', content)
content = re.sub(r'DBService::InsertActivityLog\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->InsertActivityLog(\2, \3)', content)

# Fix MapManager:: static calls
content = re.sub(r'MapManager::Init\((\w+)\)', r'\1->Init()', content)
content = re.sub(r'MapManager::StartAll\((\w+)\)', r'\1->StartAll()', content)
content = re.sub(r'MapManager::StopAll\((\w+)\)', r'\1->StopAll()', content)
content = re.sub(r'MapManager::GetMap\((\w+),\s*(\w+)\)', r'\1->GetMap(\2)', content)
content = re.sub(r'MapManager::NewDungeon\((\w+),\s*(\w+)\)', r'\1->NewDungeon(\2)', content)
content = re.sub(r'MapManager::PostMsg\((\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)', r'\1->PostMsg(\2, \3, \4, \5, \6, \7, \8)', content)

# Fix Player:: static calls (most already fine, but some remain)
content = re.sub(r'Player::initNetPacketHandlers\(\)', r'Player::initNetPacketHandlers()', content)
content = re.sub(r'Player::RecalcAttr\((\w+)\)', r'\1->RecalcAttr()', content)

# Fix Map:: static calls  
content = re.sub(r'Map::GetMapId\((\w+)\)', r'\1->GetMapId()', content)
content = re.sub(r'Map::GetRunnerId\((\w+)\)', r'\1->GetRunnerId()', content)
content = re.sub(r'Map::getRandomWalkablePositionInRegion\((\w+),\s*(\w+)\)', r'\1->getRandomWalkablePositionInRegion(\2)', content)

# Fix Player:: specific static calls with first arg as this
content = re.sub(r'Player::GetPlayerVip\((\w+)\)', r'\1->GetPlayerVip()', content)
content = re.sub(r'Player::GetPlayerDailyActivity\((\w+)\)', r'\1->GetPlayerDailyActivity()', content)
content = re.sub(r'Player::GetPlayerHuoYueDu\((\w+)\)', r'\1->GetPlayerHuoYueDu()', content)
content = re.sub(r'Player::GetOperateLimit\((\w+)\)', r'\1->GetOperateLimit()', content)
content = re.sub(r'Player::GetCharTitle\((\w+)\)', r'\1->GetCharTitle()', content)
content = re.sub(r'Player::getFamilyId\((\w+)\)', r'\1->getFamilyId()', content)
content = re.sub(r'Player::getFamilyPosition\((\w+)\)', r'\1->getFamilyPosition()', content)
content = re.sub(r'Player::getLastLoginTime\((\w+)\)', r'\1->getLastLoginTime()', content)
content = re.sub(r'Player::getUid\((\w+)\)', r'\1->getUid()', content)
content = re.sub(r'Player::getSid\((\w+)\)', r'\1->getSid()', content)
content = re.sub(r'Player::SendOtherPlayerInfo\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->SendOtherPlayerInfo(\2, \3)', content)
content = re.sub(r'Player::SendFamilyWarIcon\((\w+)\)', r'\1->SendFamilyWarIcon()', content)
content = re.sub(r'Player::InitTencentInfo\((\w+),\s*(\w+)\)', r'\1->InitTencentInfo(\2)', content)
content = re.sub(r'Player::SetActState\((\w+),\s*(\w+)\)', r'\1->SetActState(\2)', content)
content = re.sub(r'Player::SetNeedSyncAround\((\w+)\)', r'\1->SetNeedSyncAround()', content)
content = re.sub(r'Player::addNetPacket\((\w+),\s*(\w+),\s*(\w+)u?\)', r'\1->addNetPacket(\2, \3)', content)
content = re.sub(r'Player::setGateIndex\((\w+),\s*(-?\w+)\)', r'\1->setGateIndex(\2)', content)
content = re.sub(r'Player::broadcastLeave\((\w+)\)', r'\1->broadcastLeave()', content)
content = re.sub(r'Player::updateRecord\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->updateRecord(\2, \3)', content)
content = re.sub(r'Player::getRecord\((\w+),\s*(\w+)\)', r'\1->getRecord(\2)', content)
content = re.sub(r'Player::AddDaTiExp\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->AddDaTiExp(\2, \3)', content)
content = re.sub(r'Player::SendIconState\((\w+),\s*(\w+)\)', r'\1->SendIconState(\2)', content)
content = re.sub(r'Player::onLogout\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->onLogout(\2, \3)', content)
content = re.sub(r'Player::getName\(\(const Player \*const\)&(\w+)\)', r'Player::getName(nullptr, &\1)', content)
content = re.sub(r'Player::GetPassport\(\(const Player \*const\)&(\w+)\)', r'Player::GetPassport(nullptr, &\1)', content)
content = re.sub(r'Player::GetPlatform\(\(const Player \*const\)(\w+)\)', r'Player::GetPlatform(nullptr, \1)', content)

# ===== 4. Fix remaining static-like calls for other singletons =====
content = re.sub(r'TileManager::Init\((\w+)\)', r'\1->Init()', content)
content = re.sub(r'CActivityManager::Init\((\w+),\s*(\w+)\)', r'\1->Init(\2)', content)
content = re.sub(r'CActivityManager::GetCurActivityId\((\w+),\s*(\w+)\)', r'\1->GetCurActivityId(\2)', content)
content = re.sub(r'CActivityManager::GetCityWarWinner\((\w+),\s*(\w+)\)', r'\1->GetCityWarWinner(\2)', content)
content = re.sub(r'CActivityManager::UpdateActivityState\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->UpdateActivityState(\2, \3)', content)
content = re.sub(r'CGMBackstate::Init\((\w+),\s*(\w+)\)', r'\1->Init(\2)', content)
content = re.sub(r'CFestivalDoubleEleven::Init\((\w+),\s*(\w+)\)', r'\1->Init(\2)', content)
content = re.sub(r'CFestivalDoubleEleven::SendDaTiReward\((\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)', r'\1->SendDaTiReward(\2, \3, \4)', content)
content = re.sub(r'CZongHeYunYingHD::Init\((\w+),\s*(\w+)\)', r'\1->Init(\2)', content)
content = re.sub(r'CZongHeYunYingHD::UpdatePlayerInfo\((\w+),\s*(\w+),\s*(\w+)\)', r'\1->UpdatePlayerInfo(\2, \3)', content)
content = re.sub(r'CKiaFuRecharge::Init\((\w+),\s*(\w+)\)', r'\1->Init(\2)', content)
content = re.sub(r'CFestivalActivity::Init\((\w+)\)', r'\1->Init()', content)
content = re.sub(r'COpenBeta::getIconState\((\w+),\s*(\w+),\s*(\w+)\)', r'\1.getIconState(\2, \3)', content)
content = re.sub(r'CPetManager::OnUpdatePetRank\((\w+),\s*(\w+)\)', r'\1->OnUpdatePetRank(\2)', content)
content = re.sub(r'CPetManager::OnInitPetRank\((\w+),\s*(\w+)\)', r'\1->OnInitPetRank(\2)', content)
content = re.sub(r'FamilyManager::OnUpdateFamilyInfo\((\w+),\s*(\w+)\)', r'\1->OnUpdateFamilyInfo(\2)', content)
content = re.sub(r'CRankMirror::OnSocialUpdateRank\((\w+),\s*(\w+)\)', r'\1->OnSocialUpdateRank(\2)', content)
content = re.sub(r'CTimer::GetNow\((\w+)\)', r'\1->GetNow()', content)
content = re.sub(r'CPoolManager::pop<\w+>\((\w+)\)', r'\1->pop<Player>()', content)
content = re.sub(r'CPoolManager::push<\w+>\((\w+),\s*(\w+)\)', r'\1->push(\2)', content)
content = re.sub(r'CWorldBoss::GetCurBoss\((\w+)\)', r'\1->GetCurBoss()', content)

# Fix Dungeon:: static calls
content = re.sub(r'Dungeon::getDungeonId\((\w+)\)', r'\1->getDungeonId()', content)
content = re.sub(r'Dungeon::getDungeonGroupId\((\w+)\)', r'\1->getDungeonGroupId()', content)
content = re.sub(r'Dungeon::InitTeamMember\((\w+),\s*(\w+)\)', r'\1->InitTeamMember(\2)', content)
content = re.sub(r'Dungeon::start\((\w+),\s*(\w+)\)', r'\1->start(\2)', content)
content = re.sub(r'Dungeon::GetCfgDungeon\((\w+)\)', r'\1->GetCfgDungeon()', content)

# Fix Answer::DayTime static calls
content = re.sub(r'Answer::DayTime::now\(\)', r'Answer::DayTime::now()', content)
# Actually this is a valid static call. Skip.
content = re.sub(r'Answer::DayTime::dayzero\((\w+)\)', r'Answer::DayTime::dayzero(\1)', content)

# ===== 5. Fix vtable calls =====
content = re.sub(
    r'\(\*\(\(int64_t \(__fastcall \*\*\)\(Player \*\)\)player->_vptr_Entity \+ 2\)\(player\)',
    r'player->getId()',
    content
)
content = re.sub(
    r'\(\*\(\(int \(__fastcall \*\*\)\(Player \*\)\)player->_vptr_Entity \+ 9\)\(player\)',
    r'player->getLevel()',
    content
)
content = re.sub(
    r'\(\*\(\(unsigned __int8 \(__fastcall \*\*\)\(Player \*\)\)player->_vptr_Entity \+ 34\)\(player\)',
    r'player->isAlive()',
    content
)
content = re.sub(
    r'\(\*\(\(void \(__fastcall \*\*\)\(Player \*, _QWORD, _QWORD, _QWORD\)\)\*\(\(_QWORD \*\)player->_vptr_Entity \+ 35\)\(player,\s*\(unsigned int\)reason,\s*\(unsigned int\)param,\s*\w+\)',
    r'player->onDie(reason, param)',
    content
)
content = re.sub(
    r'\(\*\(\(void \(__fastcall \*\*\)\(Player \*\)\)\*\(\(_QWORD \*\)player->_vptr_Entity \+ 1\)\(player\)',
    r'/* player->vtable[1] - skip */',
    content
)
content = re.sub(
    r'\(\*\(\(void \(__fastcall \*\*\)\(Map \*, Player \*, _QWORD\)\)pMap->_vptr_Map \+ 16\)\(pMap,\s*(\w+),\s*(\w+)\)',
    r'pMap->removePlayer(\1, \2)',
    content
)
content = re.sub(
    r'\(\*\(\(void \(__fastcall \*\*\)\(Dungeon \*, Player \*, int64_t, _QWORD\)\)\*\(\(_QWORD \*\)pDungeon->_vptr_Map \+ 15\)\(pDungeon,\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'pDungeon->enterMap(\1, \2, \3)',
    content
)
# Fix Monster vtable call
content = re.sub(
    r'\(\*\(\(int64_t \(__fastcall \*\*\)\(Monster \*\)\)monster->_vptr_Entity \+ 2\)\(monster\)',
    r'monster->getId()',
    content
)
# Fix Trailer vtable call
content = re.sub(
    r'\(\*\(\(int64_t \(__fastcall \*\*\)\(Trailer \*\)\)trailer->_vptr_Entity \+ 2\)\(trailer\)',
    r'trailer->getId()',
    content
)
# Fix remaining generic vtable calls
content = re.sub(
    r'\(\*\(\(int64_t \(__fastcall \*\*\)\(Player \*\)\)\w+->_vptr_Entity \+ \d+\)\(\w+\)',
    r'/* vtable call */',
    content
)

# ===== 6. Fix undeclared variables (WOffset, proc, connid, cgindex) =====
content = re.sub(r'\bWOffset\b', r'wOffset', content)

with open("src/Game/GameService.cpp", "w") as f:
    f.write(content)

print("Second pass transformations applied successfully.")
