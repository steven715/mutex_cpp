#if !defined(WRITER_H)
#define WRITER_H

#include "Container.h"

#include <thread>
#include <chrono>

class Writer
{
private:
    std::shared_ptr<Container> m_container;
    int m_interval = 1; // milliseconds
public:
    Writer(std::shared_ptr<Container> container) : m_container(container) {};
    ~Writer() {};

    void DoWork()
    {
        extern volatile bool g_running;  // 引用全域停止標誌
        while (g_running)
        {
            const int months[] = {3, 6, 9, 12};
            int selectedMonth = months[std::rand() % 4];
            std::string id = "Obj_" + std::string(selectedMonth < 10 ? "0" : "") + std::to_string(selectedMonth);

            m_container->AddObject(id, std::rand() % 101 + 2300, std::rand() % 15 + 1);

            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }
    }
};


#endif // WRITER_H
