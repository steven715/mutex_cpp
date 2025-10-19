#!/bin/bash

# 多線程讀寫性能測試腳本
# 測試四種不同的讀寫比例場景

echo "==================================================="
echo "多線程 Mutex 性能測試"
echo "測試時間：每個場景 1 分鐘"
echo "==================================================="

# 確保程式已編譯
if [ ! -f "/root/mutex_cpp/build/mutex_example" ]; then
    echo "錯誤：程式尚未編譯，請先執行 make"
    echo "建議執行："
    echo "cd /root/mutex_cpp/build && make"
    exit 1
fi

# 創建結果目錄
RESULT_DIR="/root/mutex_cpp/benchmark_results"
mkdir -p "$RESULT_DIR"

# 獲取當前時間戳
TIMESTAMP=$(date '+%Y%m%d_%H%M%S')
SUMMARY_FILE="$RESULT_DIR/benchmark_summary_$TIMESTAMP.txt"

echo "開始測試時間：$(date)" > "$SUMMARY_FILE"
echo "====================================================" >> "$SUMMARY_FILE"

# 場景1：低寫入，高讀取 (1 Writer, 10 Readers)
echo ""
echo "🔵 場景1：低寫入高讀取 (1W:10R) - 模擬查詢密集型應用"
echo "執行中...（60秒）"
timeout --signal=INT 60s /root/mutex_cpp/build/mutex_example 1 10 > "$RESULT_DIR/scenario1_lowW_highR_$TIMESTAMP.log" 2>&1
echo "場景1：低寫入高讀取 (1W:10R)" >> "$SUMMARY_FILE"
echo "----------------------------------------" >> "$SUMMARY_FILE"
tail -n 10 "$RESULT_DIR/scenario1_lowW_highR_$TIMESTAMP.log" >> "$SUMMARY_FILE"
echo "" >> "$SUMMARY_FILE"

# 場景2：平衡讀寫 (1 Writers, 1 Readers)
echo ""
echo "🟡 場景2：平衡讀寫 (1W:1R) - 模擬平衡型應用"
echo "執行中...（60秒）"
timeout --signal=INT 60s /root/mutex_cpp/build/mutex_example 1 1 > "$RESULT_DIR/scenario2_balanced_$TIMESTAMP.log" 2>&1
echo "場景2：平衡讀寫 (1W:1R)" >> "$SUMMARY_FILE"
echo "----------------------------------------" >> "$SUMMARY_FILE"
tail -n 10 "$RESULT_DIR/scenario2_balanced_$TIMESTAMP.log" >> "$SUMMARY_FILE"
echo "" >> "$SUMMARY_FILE"

# 場景3：高寫入，低讀取 (10 Writers, 1 Reader)
echo ""
echo "🔴 場景3：高寫入低讀取 (10W:1R) - 模擬寫入密集型應用"
echo "執行中...（60秒）"
timeout --signal=INT 60s /root/mutex_cpp/build/mutex_example 10 1 > "$RESULT_DIR/scenario3_highW_lowR_$TIMESTAMP.log" 2>&1
echo "場景3：高寫入低讀取 (10W:1R)" >> "$SUMMARY_FILE"
echo "----------------------------------------" >> "$SUMMARY_FILE"
tail -n 10 "$RESULT_DIR/scenario3_highW_lowR_$TIMESTAMP.log" >> "$SUMMARY_FILE"
echo "" >> "$SUMMARY_FILE"

# 場景4：極高競爭 (10 Writers, 10 Readers)
echo ""
echo "🟠 場景4：極高競爭 (10W:10R) - 模擬高並發應用"
echo "執行中...（60秒）"
timeout --signal=INT 60s /root/mutex_cpp/build/mutex_example 10 10 > "$RESULT_DIR/scenario4_highContention_$TIMESTAMP.log" 2>&1
echo "場景4：極高競爭 (10W:10R)" >> "$SUMMARY_FILE"
echo "----------------------------------------" >> "$SUMMARY_FILE"
tail -n 10 "$RESULT_DIR/scenario4_highContention_$TIMESTAMP.log" >> "$SUMMARY_FILE"
echo "" >> "$SUMMARY_FILE"

# 完成總結
echo ""
echo "✅ 所有測試完成！"
echo ""
echo "結果文件位置："
echo "📁 詳細日誌：$RESULT_DIR/"
echo "📊 統計總結：$SUMMARY_FILE"
echo ""
echo "查看總結："
echo "cat $SUMMARY_FILE"
echo ""
echo "查看特定場景（例如場景1）："
echo "cat $RESULT_DIR/scenario1_lowW_highR_$TIMESTAMP.log"

# 顯示簡要總結
echo ""
echo "=== 快速總結 ==="
echo "場景1 (1W:10R)：查看 scenario1_lowW_highR_$TIMESTAMP.log"
echo "場景2 (1W:1R) ：查看 scenario2_balanced_$TIMESTAMP.log"
echo "場景3 (10W:1R)：查看 scenario3_highW_lowR_$TIMESTAMP.log"
echo "場景4 (10W:10R) ：查看 scenario4_highContention_$TIMESTAMP.log"
echo "==============="

echo "" >> "$SUMMARY_FILE"
echo "測試完成時間：$(date)" >> "$SUMMARY_FILE"