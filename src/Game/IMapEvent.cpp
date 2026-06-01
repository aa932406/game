#include "Game/IMapEvent.h"

#include <cstring>
#include <algorithm>
#include <string>

IMapEvent::IMapEvent()
{
    this->_vptr_IMapEvent = (int (**)(...))off_8E5490;
      std::list<CfgMapEvent>::list(&this->m_events);
}

IMapEvent::~IMapEvent()
{
    this->_vptr_IMapEvent = (int (**)(...))off_8E5490;
      std::list<CfgMapEvent>::~list(&this->m_events);
}


void IMapEvent::init(const CfgMapEventList *const events, int64_t nTime)
{

      this->m_events = events;
      for ( iter._M_node = std::list<CfgMapEvent>::begin(&this->m_events)._M_node;
            ;
            ++iter )
      {
        __x._M_node = std::list<CfgMapEvent>::end(&this->m_events)._M_node;
        if ( !iter != __x )
          break;
        if ( iter->bInitOpen )
          iter->nOpenTime = nTime;
      }
}

void IMapEvent::openEvent(int32_t nEvent, int64_t nTick)
{
    CfgMapEvent *v3; // rax
      void (__fastcall *v5)(IMapEvent *const, CfgMapEvent *); // rbx
      CfgMapEvent *v6; // rdx

      for ( iter._M_node = std::list<CfgMapEvent>::begin(&this->m_events)._M_node;
            ;
            ++iter )
      {
        __x._M_node = std::list<CfgMapEvent>::end(&this->m_events)._M_node;
        if ( !iter != __x )
          break;
        if ( iter->trigger_id == nEvent )
        {
          v3 = iter->;
          if ( !CfgMapEvent::isOpen(v3) )
          {
            iter->nOpenTime = nTick;
            v5 = (void (__fastcall *)(IMapEvent *const, CfgMapEvent *))*((int64_t *)this->_vptr_IMapEvent + 3);
            v6 = *iter;
            v5(this, v6);
          }
        }
      }
}

void IMapEvent::closeEvent(int32_t nEvent, int64_t nTick)
{
    CfgMapEvent *v3; // rax

      for ( iter._M_node = std::list<CfgMapEvent>::begin(&this->m_events)._M_node;
            ;
            ++iter )
      {
        __x._M_node = std::list<CfgMapEvent>::end(&this->m_events)._M_node;
        if ( !iter != __x )
          break;
        if ( iter->trigger_id == nEvent )
        {
          v3 = iter->;
          if ( CfgMapEvent::isOpen(v3) )
            iter->nCloseTime = nTick;
        }
      }
}

void IMapEvent::openEvents(const Int32List *const events, int64_t nTick)
{

      for ( iter._M_node = std::list<int>::begin(events)._M_node; ; ++iter )
      {
        __x._M_node = std::list<int>::end(events)._M_node;
        if ( !iter != __x )
          break;
        v3 = **iter;
        IMapEvent::openEvent(this, v3, nTick);
      }
}

void IMapEvent::closeEvents(const Int32List *const events, int64_t nTick)
{
    {

      for ( iter._M_node = std::list<int>::begin(events)._M_node; ; ++iter )
      {
        __x._M_node = std::list<int>::end(events)._M_node;
        if ( !iter != __x )
          break;
        v3 = **iter;
        IMapEvent::closeEvent(this, v3, nTick);
      }
}
