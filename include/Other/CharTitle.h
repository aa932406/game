#ifndef _CHARTITLE_H_
#define _CHARTITLE_H_

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
class CfgTitle;

class CharTitle
{
public:
    CharTitle(const CfgTitle *title);

    bool CheckJob(int32_t nJob);
    bool CheckSex(int8_t nSex);
    bool CheckPlatform(const std::string *platform);
    bool IsInParams(int32_t nParam);
    int32_t GetCardTitleParams();
    bool IsGet();
    bool Dress();
    bool UnDress();
    void Get();

private:
    const CfgTitle* cfgTitle;
    int32_t nState;
};

#endif // _CHARTITLE_H_
