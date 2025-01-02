
@echo off

echo Copy armcc.via...
copy /Y .\keil\a2\armcc.via           .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink.via...
copy /Y .\keil\a2\armlink.via         .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug.ini...
copy /Y .\keil\a2\debug.ini           .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621.sct...
copy /Y .\keil\a2\scatter_hs6621.sct  .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6621.s...
copy /Y .\keil\a2\startup_hs6621.s    .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug_flash.ini...
copy /Y .\keil\a2\debug_flash.ini           .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621_flash.sct...
copy /Y .\keil\a2\scatter_hs6621_flash.sct  .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6621_flash.s...
copy /Y .\keil\a2\startup_hs6621_flash.s    .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

goto SWITCH_END


:SWITCH_FAIL
echo !!! Switch Fail, Please close all your MDK projects !!!
pause

:SWITCH_END

