::关闭所有回显功能
@echo off

::获取当前运行的脚本文件名称，包含后缀
set Fn=%~nx0

::清除当前路径及其子目录下所有空文件
for /f "tokens=*" %%i in ('dir/s/b/ad^|sort /r') do rd "%%i"

::删除脚本文件
for %%i in (*) do (
	if "%%i" equ "%Fn%" (
		del "%%i"
	)
)