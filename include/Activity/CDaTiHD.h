#ifndef _CDATIHD_H_
#define _CDATIHD_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Activity/CActivity.h"

class Player;
class CActivity;
class CActivityMap;

class CDaTiHD : public CActivity
{
public:
    CDaTiHD(const CfgActivity* cfgActivity);
    ~CDaTiHD();

    void reset();
    void CheckActivity();
    void RandAnswer();
    void RandomQuestions();
    void SendReadyStartDaTiHD();
    void SendEndDaTiHD();
    void SendStartDaTiHD();
    void SendQuestionsInfo(int32_t NowSeconds);
    void broadcastReady();
    void broadcastStart();

private:
        int32_t m_DaTiIndex;
        // TODO: 确认类型 m_IsSendReadyStartToCs
        // TODO: 确认类型 m_LastSendQuestions
        // TODO: 确认类型 m_QuestionsVct
        int32_t m_RandAnswerDaTiIndex;
        int32_t m_nState;
};

#endif // _CDATIHD_H_

