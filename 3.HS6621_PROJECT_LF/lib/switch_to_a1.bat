
@echo off

echo Copy armcc.via...
copy /Y .\keil\a1\armcc.via           .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink.via...
copy /Y .\keil\a1\armlink.via         .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug.ini...
copy /Y .\keil\a1\debug.ini           .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6620.sct...
copy /Y .\keil\a1\scatter_hs6621.sct  .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6620.s...
copy /Y .\keil\a1\startup_hs6621.s    .\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

goto SWITCH_END


:SWITCH_FAIL
echo !!! Switch Fail, Please close all your MDK projects !!!
pause

:SWITCH_END

