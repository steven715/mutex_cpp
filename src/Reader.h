#if !defined(READER_H)
#define READER_H

#include "Container.h"
#include <iostream>
#include <thread>
#include <chrono>

class Reader
{
private:
    std::shared_ptr<Container> m_container;
    int m_interval = 1; // milliseconds

public:
    Reader(std::shared_ptr<Container> container) : m_container(container) {};
    ~Reader() {};

    void DoWork()
    {
        extern volatile bool g_running;  // 引用全域停止標誌
        while (g_running)
        {
            const int months[] = {3, 6, 9, 12};
            int selectedMonth = months[std::rand() % 4];
            std::string id = "Obj_" + std::string(selectedMonth < 10 ? "0" : "") + std::to_string(selectedMonth);

            auto obj = m_container->GetObject(id);
            if (obj)
            {
                // std::cout << "Read Object ID: " << obj->GetID()
                //           << ", Value: " << obj->GetValue()
                //           << ", Number: " << obj->GetNumber() << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }
    }
};



#endif // READER_H
