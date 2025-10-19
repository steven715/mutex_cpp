#include <iostream>
#include <vector>
#include <signal.h>
#include "Container.h"
#include "Writer.h"
#include "Reader.h"

// 全域變數用於優雅關閉
volatile bool g_running = true;
std::shared_ptr<Container> g_container;

void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\n收到 SIGINT 信號 (Ctrl+C)，正在統計性能..." << std::endl;
    } else if (signal == SIGTERM) {
        std::cout << "\n收到 SIGTERM 信號 (timeout)，正在統計性能..." << std::endl;
    }
    g_running = false;
}

int main(int argc, char const *argv[])
{
    // 設定信號處理器
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGTERM, signalHandler);  // timeout 預設信號
    
    auto container = std::make_shared<Container>();
    g_container = container;

    // 設定線程數量（可從命令列參數指定或使用預設值）
    int writerCount = 2;  // 預設 Writer 線程數量
    int readerCount = 3;  // 預設 Reader 線程數量

    // 如果有命令列參數，則使用參數指定的數量
    if (argc >= 3) {
        writerCount = std::atoi(argv[1]);
        readerCount = std::atoi(argv[2]);
        
        // 確保數量在合理範圍內
        if (writerCount <= 0 || writerCount > 10) writerCount = 2;
        if (readerCount <= 0 || readerCount > 10) readerCount = 3;
    }
    else if (argc == 1) {
        // 互動式輸入
        std::cout << "請輸入 Writer 線程數量 (1-10, 預設 2): ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            int temp = std::atoi(input.c_str());
            if (temp > 0 && temp <= 10) writerCount = temp;
        }

        std::cout << "請輸入 Reader 線程數量 (1-10, 預設 3): ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            int temp = std::atoi(input.c_str());
            if (temp > 0 && temp <= 10) readerCount = temp;
        }
    }

    std::cout << "啟動 " << writerCount << " 個 Writer 線程和 " 
              << readerCount << " 個 Reader 線程..." << std::endl;

    // 創建 Writer 物件和線程
    std::vector<std::unique_ptr<Writer>> writers;
    std::vector<std::thread> writerThreads;
    
    for (int i = 0; i < writerCount; ++i) {
        writers.push_back(std::make_unique<Writer>(container));
        writerThreads.emplace_back(&Writer::DoWork, writers[i].get());
        std::cout << "Writer 線程 " << (i + 1) << " 已啟動" << std::endl;
    }

    // 創建 Reader 物件和線程
    std::vector<std::unique_ptr<Reader>> readers;
    std::vector<std::thread> readerThreads;
    
    for (int i = 0; i < readerCount; ++i) {
        readers.push_back(std::make_unique<Reader>(container));
        readerThreads.emplace_back(&Reader::DoWork, readers[i].get());
        std::cout << "Reader 線程 " << (i + 1) << " 已啟動" << std::endl;
    }

    // 等待所有 Writer 線程完成
    for (auto& thread : writerThreads) {
        thread.join();
    }

    // 等待所有 Reader 線程完成
    for (auto& thread : readerThreads) {
        thread.join();
    }

    // 顯示最終統計結果
    container->PrintStats();

    return 0;
}
