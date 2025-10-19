# Mutex Cpp

這是一個探索C++ 23 中各類鎖性能的比較專案，主要比較一般的互斥鎖 (mutex), 讀寫鎖 (shared_mutex), 自旋鎖 (自己實作的spin lock)。

## 場景說明

以一個類似行情物件為基礎，我會定義一個容器類別來管理行情物件，同時定義一個Reader類別，來向容器讀取這些物件，並另外再定義一個Writer類別來不斷向容器寫入行情。

基於上述場景的定義，我就能藉由擴增Reader及Writer的數量，來模擬不同的讀/寫比例的場景了。

## 建置

本專案使用 cmake 建置。

```sh
mkdir build && cd build
cmake ..
make
```

### 選擇鎖的實作

目前有提供3種鎖的實作，使用的鎖能透過 `LOCK_MUTEX_IMP` 來做切換，切換後再做建置即可切換實作了。

```cpp
// Container.h
#define LOCK_MUTEX_IMP 2
#define LOCK_MUTEX 1
#define LOCK_SPIN 2
#define LOCK_RW 3
```

## 執行

輸入帶參數能指定當前的寫者數量以及讀者數量。

```sh
# 1個寫者 3個讀者
./mutex_example 1 3

# 也可以不帶，程序會再要求輸入寫者與讀者的數量
./mutex_example
```

## 性能測試

- 目前測試場景有以下4種: 低寫入高讀取 (1W:10R), 平衡讀寫 (1W:1R), 高寫入低讀取 (10W:1R), 極高競爭 (10W:10R)
- `quick_test.sh`: 快速執行各場景性能測試的腳本，每個場景執行10秒
- `run_benchmark.sh`: 執行各場景性能測試的腳本，並輸出結果到指定資料夾下 (benchmark_results)，每個場景執行1分鐘

### 備註

- 目前讀者、寫者的操作頻率都是 1 毫秒 (millisecond)，此部分能透過 `Reader.h`, `Writer.h` 進行上下調整

## 未來擴充

- 加入無鎖資料結構
- 性能測試要再補上程序對CPU, Mem的使用分析
- 場景的延伸與擴充
