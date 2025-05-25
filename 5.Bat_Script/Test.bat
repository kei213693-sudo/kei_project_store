@echo off
::强制终端使用 GBK 编码（代码页 936）
chcp 936 > nul

echo === BAT 环境测试 ===
echo OS Version: %OS%
echo Current User: %USERNAME%

pause
exit