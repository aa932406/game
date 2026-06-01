#include "Game/CTimer.h"

#include <cstring>
#include <algorithm>

CTimer::CTimer()
{
    Answer::Thread::Thread(this);
    // 虚函数表由编译器自动设置
    this->m_nTick = Answer::DayTime::tick();
    this->m_nNow = Answer::DayTime::now();
    Answer::DayTime::localnow(&this->m_tmLocalNow);
    this->m_tmLastLocalNow = this->m_tmLocalNow;
    this->m_nextAnnoucement = 0;
    this->m_nWeekDay = 0;
    Answer::Mutex::Mutex(&this->m_timeLock);
    this->m_nWeekDay = (this->m_tmLocalNow.tm_wday + 6) % 7 + 1;
}

CTimer::~CTimer()
{
    // 虚函数表由编译器自动处理
    Answer::Mutex::~Mutex(&this->m_timeLock);
    Answer::Thread::~Thread(this);
}


void CTimer::run()
{
    tm NowTm;
    int64_t CurTick;

    this->m_bRunning = 1;
    while ( this->m_bRunning )
    {
        CurTick = CTimer::GetTick(this);
        CTimer::GetLocalNow(&NowTm, this);
        CTimer::updateTime(this);
        CTimer::checkNewMinute(this, &NowTm);
        CActivityManager::OnUpdate(Answer::Singleton<CActivityManager>::instance(), CurTick);
        CGMBackstate::OnUpdate(Answer::Singleton<CGMBackstate>::instance());
        CWorldBoss::Update(Answer::Singleton<CWorldBoss>::instance(), CurTick);
        Answer::DayTime::sleep(10);
    }
}

void CTimer::Start()
{
    tm nowTm;
    CTimer *thisa = this;

    this->m_bRunning = 1;
    CTimer::GetLocalNow(&nowTm, this);
    thisa->m_tmLastLocalNow = nowTm;
    thisa->m_nextAnnoucement = 0;
    Answer::Thread::start(thisa);
}

void CTimer::Stop()
{
    this->m_bRunning = 0;
    Answer::Thread::join(this);
}

int64_t CTimer::GetTick()
{
    int64_t m_nTick;
    Answer::MutexGuard lock;
    Answer::MutexGuard::MutexGuard(&lock, &this->m_timeLock);
    m_nTick = this->m_nTick;
    Answer::MutexGuard::~MutexGuard(&lock);
    return m_nTick;
}

int32_t CTimer::GetNow()
{
    int32_t m_nNow;
    Answer::MutexGuard lock;
    Answer::MutexGuard::MutexGuard(&lock, &this->m_timeLock);
    m_nNow = this->m_nNow;
    Answer::MutexGuard::~MutexGuard(&lock);
    return m_nNow;
}

void CTimer::GetLocalNow(tm *__return_ptr retstr)
{
    Answer::MutexGuard lock;
    Answer::MutexGuard::MutexGuard(&lock, &this->m_timeLock);
    *retstr = this->m_tmLocalNow;
    Answer::MutexGuard::~MutexGuard(&lock);
    return retstr;
}

bool CTimer::BetweenDate(int32_t begin_date, int32_t end_date)
{
    int date;
    date = 10000 * (this->m_tmLocalNow.tm_year + 1900)
         + 100 * (this->m_tmLocalNow.tm_mon + 1)
         + this->m_tmLocalNow.tm_mday;
    return date >= begin_date && date <= end_date;
}

void CTimer::updateTime()
{
    GameService *v1;
    tm localNow;
    Answer::MutexGuard lock;
    int64_t nTick;
    int32_t nNow;

    nTick = Answer::DayTime::tick();
    nNow = Answer::DayTime::now();
    Answer::DayTime::localnow(&localNow);

    if ( CTimer::isRightLocalNow(this, &localNow) )
    {
        if ( this->m_tmLocalNow.tm_sec != localNow.tm_sec && !(localNow.tm_sec % 15) )
        {
            v1 = Answer::Singleton<GameService>::instance();
            GameService::onUpdatePay(v1);
        }
        Answer::MutexGuard::MutexGuard(&lock, &this->m_timeLock);
        this->m_nTick = nTick;
        this->m_nNow = nNow;
        this->m_tmLocalNow = localNow;
        Answer::MutexGuard::~MutexGuard(&lock);
    }
}

bool CTimer::isRightLocalNow(const tm *const localnow)
{
    if ( localnow->tm_year < this->m_tmLocalNow.tm_year )
        return 0;
    if ( localnow->tm_year != this->m_tmLocalNow.tm_year )
        return 1;
    if ( localnow->tm_yday < this->m_tmLocalNow.tm_yday )
        return 0;
    if ( localnow->tm_yday != this->m_tmLocalNow.tm_yday )
        return 1;
    if ( localnow->tm_hour < this->m_tmLocalNow.tm_hour )
        return 0;
    if ( localnow->tm_hour != this->m_tmLocalNow.tm_hour )
        return 1;
    if ( localnow->tm_min < this->m_tmLocalNow.tm_min )
        return 0;
    return localnow->tm_min != this->m_tmLocalNow.tm_min || localnow->tm_sec >= this->m_tmLocalNow.tm_sec;
}

void CTimer::checkNewMinute(const tm *const localnow)
{
    if ( localnow->tm_yday != this->m_tmLastLocalNow.tm_yday )
    {
        Answer::Logger::print(
            Answer::LogLevel::LOG_LEVEL_INFO,
            "CTimer::onDaySwitch() nowday=%d-%d %d:%d:%d, lastDay=%d-%d %d:%d:%d\n",
            localnow->tm_year,
            localnow->tm_yday,
            localnow->tm_hour,
            localnow->tm_min,
            localnow->tm_sec,
            this->m_tmLastLocalNow.tm_year,
            this->m_tmLastLocalNow.tm_yday,
            this->m_tmLastLocalNow.tm_hour,
            this->m_tmLastLocalNow.tm_min,
            this->m_tmLastLocalNow.tm_sec);
        CTimer::onDaySwitch(this);
    }
    if ( localnow->tm_min != this->m_tmLastLocalNow.tm_min )
        CTimer::onNewMinute(this, localnow->tm_min + 60 * localnow->tm_hour);
    if ( localnow->tm_wday != this->m_tmLastLocalNow.tm_wday )
        CTimer::onWeekSwitch(this);
    this->m_tmLastLocalNow = *localnow;
}

void CTimer::onNewMinute(int32_t nMinute)
{
    GameService::onNewMinuteCome(Answer::Singleton<GameService>::instance(), nMinute);
    CUniteServer::OnNewMinute(Answer::Singleton<CUniteServer>::instance(), nMinute);
    CFestivalDoubleEleven::OnNewMinute(Answer::Singleton<CFestivalDoubleEleven>::instance(), nMinute);
    CFestivalActivity::OnNewMinute(Answer::Singleton<CFestivalActivity>::instance(), nMinute);
    CZongHeYunYingHD::OnNewMinute(Answer::Singleton<CZongHeYunYingHD>::instance(), nMinute);
    CKiaFuRecharge::OnNewMinute(Answer::Singleton<CKiaFuRecharge>::instance(), nMinute);
    COpenBeta::OnNewMinute(Answer::Singleton<COpenBeta>::instance(), nMinute);
    COutLinkFestival::OnNewMinute(Answer::Singleton<COutLinkFestival>::instance(), nMinute);
    CGuiGuDaoRen::OnNewMinute(Answer::Singleton<CGuiGuDaoRen>::instance(), nMinute);
}

void CTimer::onWeekSwitch()
{
    // 空实现 - 为保留的虚函数接口
}

void CTimer::onDaySwitch()
{
    this->m_nWeekDay = (this->m_tmLocalNow.tm_wday + 6) % 7 + 1;
    GameService::OnDaySwitch(Answer::Singleton<GameService>::instance());
    CUniteServer::OnDaySwitch(Answer::Singleton<CUniteServer>::instance());
    CFestivalDoubleEleven::OnDaySwitch(Answer::Singleton<CFestivalDoubleEleven>::instance());
    CFestivalActivity::OnDaySwitch(Answer::Singleton<CFestivalActivity>::instance());
    CZongHeYunYingHD::OnDaySwitch(Answer::Singleton<CZongHeYunYingHD>::instance());
    CKiaFuRecharge::OnDaySwitch(Answer::Singleton<CKiaFuRecharge>::instance());
    COpenBeta::OnDaySwitch(Answer::Singleton<COpenBeta>::instance());
    COutLinkFestival::OnDaySwitch(Answer::Singleton<COutLinkFestival>::instance());
    CGuiGuDaoRen::OnDaySwitch(Answer::Singleton<CGuiGuDaoRen>::instance());
    if ( !Answer::CheckKey::CheckKeyValid() )
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR, "KEY\n");
        exit(1);
    }
}
