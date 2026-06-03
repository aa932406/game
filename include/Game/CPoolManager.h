#ifndef _CPOOLMANAGER_H_
#define _CPOOLMANAGER_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class CPoolManager
{
public:
    static CPoolManager* GetInstance();

    void Init();

    template<typename T>
    T* pop()
    {
        return new T();
    }

    template<typename T>
    T* Pop()
    {
        return new T();
    }

    template<typename T>
    void Push(T* obj)
    {
        delete obj;
    }

    template<typename T>
    void regPool(int32_t minCount, int32_t maxCount) { (void)minCount; (void)maxCount; }
};

#endif // _CPOOLMANAGER_H_