#include <iostream>
#include <vector>
#include "Container.h"
#include "Writer.h"
#include "Reader.h"

int main(int argc, char const *argv[])
{
    auto container = std::make_shared<Container>();

    // 設定線程數量
    const int writerThreadCount = 2;
    const int readerThreadCount = 3;

    std::cout << "使用單一物件方法啟動 " << writerThreadCount 
              << " 個 Writer 線程和 " << readerThreadCount << " 個 Reader 線程..." << std::endl;

    // 方法1：單一 Writer 物件，多個線程
    Writer writer(container);
    Reader reader(container);

    std::vector<std::thread> writerThreads;
    std::vector<std::thread> readerThreads;

    // 創建多個線程執行同一個 Writer 物件的 DoWork
    for (int i = 0; i < writerThreadCount; ++i) {
        writerThreads.emplace_back(&Writer::DoWork, &writer);
        std::cout << "Writer 線程 " << (i + 1) << " 已啟動" << std::endl;
    }

    // 創建多個線程執行同一個 Reader 物件的 DoWork
    for (int i = 0; i < readerThreadCount; ++i) {
        readerThreads.emplace_back(&Reader::DoWork, &reader);
        std::cout << "Reader 線程 " << (i + 1) << " 已啟動" << std::endl;
    }

    // 等待所有線程完成
    for (auto& thread : writerThreads) {
        thread.join();
    }
    for (auto& thread : readerThreads) {
        thread.join();
    }

    return 0;
}