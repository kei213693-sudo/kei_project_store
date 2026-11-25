@echo off
REM 执行Python脚本后自动关闭窗口
setlocal

REM 获取当前脚本所在目录
set "current_dir=%~dp0"

REM 执行Python脚本
python "%current_dir%Test.py"

REM 检查Python脚本的退出代码
if %errorlevel% equ 0 (
    echo 执行成功，窗口将自动关闭...
    timeout /t 2 /nobreak >nul
    exit
) else (
    echo 执行失败，按任意键关闭窗口...
    pause
    exit
)

endlocal