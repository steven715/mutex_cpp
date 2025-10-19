#include "Container.h"
#include <iostream>
#include <iomanip>

Container::Container()
{
}

Container::~Container()
{
    m_objects.clear();
}

void Container::AddObject(const std::string &id, double value, double number)
{
    auto start = std::chrono::high_resolution_clock::now();

    {
#if LOCK_MUTEX_IMP == LOCK_MUTEX
        std::lock_guard<std::mutex> lock(m_mtx);
#elif LOCK_MUTEX_IMP == LOCK_SPIN
        std::lock_guard<spin_lock> lock(m_spinlock);
#elif LOCK_MUTEX_IMP == LOCK_RW
        std::unique_lock<std::shared_mutex> lock(m_rwlock);
#endif
        m_objects[id] = std::make_shared<Object>(id, value, number);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    m_writeCount++;
    m_totalWriteTime += duration;
}

void Container::RemoveObject(const std::string &id)
{
    auto start = std::chrono::high_resolution_clock::now();

    {
#if LOCK_MUTEX_IMP == LOCK_MUTEX
        std::lock_guard<std::mutex> lock(m_mtx);
#elif LOCK_MUTEX_IMP == LOCK_SPIN
        std::lock_guard<spin_lock> lock(m_spinlock);
#elif LOCK_MUTEX_IMP == LOCK_RW
        std::unique_lock<std::shared_mutex> lock(m_rwlock);
#endif
        m_objects.erase(id);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    m_writeCount++;
    m_totalWriteTime += duration;
}

void Container::PrintStats() const
{
    std::cout << "\n=== Container 性能統計 ===" << std::endl;
    std::cout << "讀取次數: " << GetReadCount() << std::endl;
    std::cout << "寫入次數: " << GetWriteCount() << std::endl;
    std::cout << "平均讀取時間: " << std::fixed << std::setprecision(2)
              << GetAverageReadTime() << " 微秒" << std::endl;
    std::cout << "平均寫入時間: " << std::fixed << std::setprecision(2)
              << GetAverageWriteTime() << " 微秒" << std::endl;

    long long totalOps = GetReadCount() + GetWriteCount();
    if (totalOps > 0)
    {
        double readRatio = static_cast<double>(GetReadCount()) / totalOps * 100;
        double writeRatio = static_cast<double>(GetWriteCount()) / totalOps * 100;
        std::cout << "讀寫比例: " << std::fixed << std::setprecision(1)
                  << readRatio << "% 讀取, " << writeRatio << "% 寫入" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

ObjectPtr Container::GetObject(const std::string &id)
{
    ObjectPtr result = nullptr;
    auto start = std::chrono::high_resolution_clock::now();

    {
#if LOCK_MUTEX_IMP == LOCK_MUTEX
        std::lock_guard<std::mutex> lock(m_mtx);
#elif LOCK_MUTEX_IMP == LOCK_SPIN
        std::lock_guard<spin_lock> lock(m_spinlock);
#elif LOCK_MUTEX_IMP == LOCK_RW
        std::shared_lock<std::shared_mutex> lock(m_rwlock);
#endif
        auto it = m_objects.find(id);
        result = (it != m_objects.end()) ? it->second : nullptr;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    m_readCount++;
    m_totalReadTime += duration;

    return result;
}
