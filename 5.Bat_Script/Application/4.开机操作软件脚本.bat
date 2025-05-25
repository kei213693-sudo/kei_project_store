::关闭所有指令回显
@echo off

for /f "tokens=2 delims==" %%i in ('wmic path win32_localtime get dayofweek /value ^| findstr "="') do (
    for /f "delims=" %%a in ("%%i") do (
		::if或实现，如果是[1,5]就打开工作日需要的软件
        if %%a lss 1 (
		    goto necessary
        )
		if %%a gtr 5 (
            goto necessary
        )
    )
)

::工作日才打开的软件
start "" "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\有道\网易有道翻译\网易有道翻译"
if %errorlevel% neq 0 (
	echo 网易有道翻译无法打开，请检查路径是否正确
	pause
)

start "" "C:\ProgramData\Microsoft\Windows\Start Menu\企业微信"
if %errorlevel% neq 0 (
	echo 企业微信无法打开，请检查路径是否正确
	pause
)

::VS Code打开会有命令行“update#setState”告警，需要手动关闭，就不妨再这里打开了

::每天都要打开的软件
:necessary
start "" "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\印象笔记\印象笔记"
if %errorlevel% neq 0 (
	echo 印象笔记无法打开，请检查路径是否正确
	pause
)

start "" "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\微信\微信"
if %errorlevel% neq 0 (
	echo 微信无法打开，请检查路径是否正确
	pause
)

start "" "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Onenote"
if %errorlevel% neq 0 (
	echo Onenote无法打开，请检查路径是否正确
	pause
)
exit



