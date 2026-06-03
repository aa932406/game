#ifndef COMMON_H
#define COMMON_H

// ============================================================
// Unified common header — include this in .cpp files
// to fix "incomplete type" errors project-wide.
// ============================================================

// --- System headers ---
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <climits>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <mutex>
#include <atomic>
#include <chrono>

// --- Answer framework core ---
#include "Answer/NetPacket.h"
#include "Answer/Singleton.h"
#include "Answer/Logger.h"
#include "Answer/DayTime.h"
#include "Answer/Mutex.h"
#include "Answer/Random.h"
#include "Answer/MD5.h"
#include "Answer/StringUtility.h"

// --- Common types ---
#include "Common/CommonTypes.h"
#include "Common/EntityType.h"
#include "Common/Random.h"

// --- Config (structs defined in CfgData.h) ---
#include "Config/CfgData.h"
#include "Config/CfgActivity.h"
#include "Config/CfgMap.h"
#include "Config/CfgDungeon.h"
#include "Config/CfgSysMail.h"
#include "Config/CfgSkillTable.h"

// --- Game core ---
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/MapManager.h"
#include "Game/Dungeon.h"
#include "Game/Monster.h"
#include "Game/Unit.h"
#include "Game/CObjPet.h"
#include "Game/CPoolManager.h"
#include "Game/CTimer.h"
#include "Game/IMapEvent.h"
#include "Game/StaticObj.h"

// --- Database ---
#include "Database/DBService.h"
#include "Database/PlayerDBData.h"

// --- Other essential ---
#include "Other/User.h"
#include "Other/CVip.h"
#include "Other/CHuoYueDu.h"

// --- Network ---
#include "Network/ConnStubs.h"

#endif // COMMON_H
