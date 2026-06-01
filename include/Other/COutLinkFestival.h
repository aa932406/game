#ifndef _COUTLINKFESTIVAL_H_
#define _COUTLINKFESTIVAL_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;

class COutLinkFestival
{
public:
    COutLinkFestival();
    ~COutLinkFestival();

    void OnNewMinute(int32_t nMinute);
    void Init(int32_t line);
    void GetIconState(IconStateList * IconList);
    void checkIconState();

private:
        // TODO: 确认类型 m_lOutLinkIcons
        // TODO: 确认类型 m_lock
};

#endif // _COUTLINKFESTIVAL_H_
