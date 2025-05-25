::关闭所有回显功能
@echo off

::获取当前运行的脚本文件名称，包含后缀
set Fn=%~nx0

::压缩包类型，7z支持“7z、tar、wim、zip”四种
set Type=.7z

::设置压缩密码
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
 
::-mhe=on  让文件名也加密
::-mx 压缩等级 0-9之间选择[0仅仅是复制]
::压缩包名称不要文件后缀可以在倒数第一个i前面加上~，倒数第2个i前面加上~n
::最后一个参数不加命令窗口显示很久，耗时长，为什么？

::压缩文件
set Out=./
for %%i in (*) do (
	if "%%i" neq "%Fn%" (
		"%Sz%" a -t7z -p%Password% -mhe=on -mx0 "%%i%Type%" -o"%Out%" "%%i"
	)
)

::压缩文件夹
set In=./*
set Out=./*
for /d %%i in ("%In%") do (
	"%Sz%" a -t7z -p%Password% -mhe=on -mx0 "%%i%Type%" -o"%Out%" "%%i"
)

::删除脚本文件
for %%i in (*) do (
	if "%%i" equ "%Fn%" (
		del "%%i"
	)
)