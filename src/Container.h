#if !defined(CONTAINER_H)
#define CONTAINER_H

#define LOCK_MUTEX_IMP 1
#define LOCK_MUTEX 1
#define LOCK_SPIN 2
#define LOCK_RW 3

#include "Object.h"
#include "Spin_Lock.h"

#include <map>
#include <memory>
#include <atomic>
#include <chrono>

#include <mutex>
#include <shared_mutex>

typedef std::shared_ptr<Object> ObjectPtr;

class Container
{
private:
    std::map<std::string, ObjectPtr> m_objects = std::map<std::string, ObjectPtr>();

    // 一般鎖
    std::mutex m_mtx;

    // 自旋鎖
    spin_lock m_spinlock;

    // 讀寫鎖
    std::shared_mutex m_rwlock;

    // 性能統計（線程安全）
    std::atomic<long long> m_readCount{0};
    std::atomic<long long> m_writeCount{0};
    std::atomic<long long> m_totalReadTime{0};  // 微秒
    std::atomic<long long> m_totalWriteTime{0}; // 微秒

public:
    Container();
    ~Container();

    void AddObject(const std::string &id, double value, double number);
    void RemoveObject(const std::string &id);
    ObjectPtr GetObject(const std::string &id);

    // 性能統計方法
    long long GetReadCount() const { return m_readCount.load(); }
    long long GetWriteCount() const { return m_writeCount.load(); }
    double GetAverageReadTime() const
    {
        long long reads = m_readCount.load();
        return reads > 0 ? static_cast<double>(m_totalReadTime.load()) / reads : 0.0;
    }
    double GetAverageWriteTime() const
    {
        long long writes = m_writeCount.load();
        return writes > 0 ? static_cast<double>(m_totalWriteTime.load()) / writes : 0.0;
    }
    void ResetStats()
    {
        m_readCount = 0;
        m_writeCount = 0;
        m_totalReadTime = 0;
        m_totalWriteTime = 0;
    }
    void PrintStats() const;
};

#endif // CONTAINER_H
