#ifndef _RWLOCK_H_
#define _RWLOCK_H_

namespace Answer {

// 读写锁（简化桩）
class RwLock {
public:
    RwLock() = default;
    ~RwLock() = default;
    RwLock(const RwLock&) = delete;
    RwLock& operator=(const RwLock&) = delete;
};

// 读锁保护
class RwLockRdGuard {
public:
    RwLockRdGuard() : m_lock(nullptr) {}
    explicit RwLockRdGuard(RwLock* lock) : m_lock(lock) {}
    ~RwLockRdGuard() = default;
private:
    RwLock* m_lock;
};

} // namespace Answer

#endif // _RWLOCK_H_
