#ifndef _RWLOCK_H_
#define _RWLOCK_H_

#include <mutex>

namespace Answer {

// 读写锁（简化桩）
class RwLock {
public:
    RwLock() = default;
    ~RwLock() = default;
    RwLock(const RwLock&) = delete;
    RwLock& operator=(const RwLock&) = delete;

    // 读锁 (简化桩)
    class ReadLock {
    public:
        explicit ReadLock(RwLock* lock) : m_lock(lock) { (void)m_lock; }
    private:
        RwLock* m_lock;
    };

    // 写锁 (简化桩)
    class WriteLock {
    public:
        explicit WriteLock(RwLock* lock) : m_lock(lock) { (void)m_lock; }
    private:
        RwLock* m_lock;
    };
};

// 读锁保护
class RwLockRdGuard {
public:
    RwLockRdGuard() : m_lock(nullptr) {}
    explicit RwLockRdGuard(RwLock* lock) : m_lock(lock) {}
    explicit RwLockRdGuard(std::mutex* lock) : m_lock(reinterpret_cast<RwLock*>(lock)) {}
    ~RwLockRdGuard() = default;
private:
    RwLock* m_lock;
};

// 写锁保护 (简化桩)
class RwLockWrGuard {
public:
    RwLockWrGuard() : m_lock(nullptr) {}
    explicit RwLockWrGuard(RwLock* lock) : m_lock(lock) {}
    ~RwLockWrGuard() = default;
private:
    RwLock* m_lock;
};

// 内嵌类别名 (用于 CExtCharDepotManager.cpp)
class RwLockReadLock {
public:
    explicit RwLockReadLock(RwLock* lock) : m_lock(lock) { (void)m_lock; }
private:
    RwLock* m_lock;
};

class RwLockWriteLock {
public:
    explicit RwLockWriteLock(RwLock* lock) : m_lock(lock) { (void)m_lock; }
private:
    RwLock* m_lock;
};

} // namespace Answer

#endif // _RWLOCK_H_
