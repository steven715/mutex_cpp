#!/bin/bash

# 快速測試腳本（每個場景10秒）

echo "快速多線程性能測試（每場景10秒）"
echo "================================="

# 確保程式已編譯
cd /root/mutex_cpp/build && make -j$(nproc)

echo ""
echo "🔵 場景1：低寫入高讀取 (1W:10R)"
timeout --signal=INT 10s ./mutex_example 1 10 > quick_test_1w10r.log 2>&1 &
SCENARIO1_PID=$!

echo "🟡 場景2：平衡讀寫 (1W:1R)"  
timeout --signal=INT 10s ./mutex_example 1 1 > quick_test_1w1r.log 2>&1 &
SCENARIO2_PID=$!

echo "🔴 場景3：高寫入低讀取 (10W:1R)"
timeout --signal=INT 10s ./mutex_example 10 1 > quick_test_10w1r.log 2>&1 &
SCENARIO3_PID=$!

echo "🟠 場景4：極高競爭 (10W:10R)"
timeout --signal=INT 10s ./mutex_example 10 10 > quick_test_10w10r.log 2>&1 &
SCENARIO4_PID=$!

# 等待所有測試完成
wait $SCENARIO1_PID $SCENARIO2_PID $SCENARIO3_PID $SCENARIO4_PID

echo ""
echo "✅ 測試完成！查看結果："
echo ""

echo "=== 場景1 (1W:10R) ==="
tail -n 8 quick_test_1w10r.log | head -n 7

echo ""
echo "=== 場景2 (1W:1R) ==="
tail -n 8 quick_test_1w1r.log | head -n 7

echo ""
echo "=== 場景3 (10W:1R) ==="
tail -n 8 quick_test_10w1r.log | head -n 7

echo ""
echo "=== 場景4 (10W:10R) ==="
tail -n 8 quick_test_10w10r.log | head -n 7