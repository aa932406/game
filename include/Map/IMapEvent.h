#pragma once
#include <cstdint>
#include <list>

class CfgMapEvent;

class IMapEvent
{
public:
    IMapEvent();
    virtual ~IMapEvent();

    virtual void init(const std::list<CfgMapEvent> *events, int64_t nTime);
    virtual void Clear();
    virtual void openEvent(int32_t eventId);
    virtual void closeEvent(int32_t eventId);
    virtual void openEvents(int32_t *eventIds, int32_t count);
    virtual void closeEvents(int32_t *eventIds, int32_t count);

protected:
    std::list<CfgMapEvent> m_events;
};
