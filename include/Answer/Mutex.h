#ifndef _MUTEX_H_
#define _MUTEX_H_

namespace Answer {

class Mutex {
public:
    Mutex() {}
    ~Mutex() {}
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
};

class MutexGuard {
public:
    MutexGuard() : m_mutex(nullptr) {}
    MutexGuard(Mutex* mutex) : m_mutex(mutex) { (void)mutex; }
    ~MutexGuard() {}
private:
    Mutex* m_mutex;
};

} // namespace Answer

#endif // _MUTEX_H_
