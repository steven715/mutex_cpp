cmake_minimum_required(VERSION 3.16)

# 專案設定
project(mutex_cpp LANGUAGES CXX)

# 使用 C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# 找到 pthread (Threads)
find_package(Threads REQUIRED)

set(SOURCE_FILES
    src/Container.cpp
    src/main.cpp
)

# 範例可執行檔（請自行在 src/main.cpp 新增程式碼）
add_executable(mutex_example ${SOURCE_FILES})

# 連結 pthread
target_link_libraries(mutex_example PRIVATE Threads::Threads)