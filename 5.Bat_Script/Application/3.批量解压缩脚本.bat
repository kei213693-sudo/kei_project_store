::关闭所有回显功能
@echo off

::获取当前运行的脚本文件名称，包含后缀
set Fn=%~nx0

::解压缩密码
set Password=3xYkSh

::7z的程序安装路径
for %%i in (7z.exe) do (
	set Sz=%%~$path:i
)
::没找到就退出不执行后续操作
if not defined Sz (
	echo 未找到7z.exe路径，请检查path是否设置
	pause
	exit
)

::遍历当前目录，解压压缩包
::压缩包类型，7z支持“7z、tar、wim、zip”四种
set Out=./
for %%i in (*.7z,*.zip,*.tar,*.wim) do (
	"%Sz%" x "%%i" -p"%Password%" -o"%Out%"
)

::删除压缩包
set In=./
for /r "%In%" %%i in (*.7z,*.zip,*.tar,*.wim) do (
	del "%%i"
)

::删除脚本文件
for %%i in (*) do (
	if "%%i" equ "%Fn%" (
		del "%%i"
	)
)