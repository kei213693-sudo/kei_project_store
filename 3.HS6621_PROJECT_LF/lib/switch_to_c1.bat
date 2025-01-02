
@echo off

set base=%~dp0
set ver=c1

echo Copy armcc.via...
copy /Y %base%\keil\%ver%\armcc.via           %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armcc_allroles.via...
copy /Y %base%\keil\%ver%\armcc_allroles.via  %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink.via...
copy /Y %base%\keil\%ver%\armlink.via         %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink_allroles.via...
copy /Y %base%\keil\%ver%\armlink_allroles.via %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug.ini...
copy /Y %base%\keil\%ver%\debug.ini           %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621.sct...
copy /Y %base%\keil\%ver%\scatter_hs6621.sct  %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6621.s...
copy /Y %base%\keil\%ver%\startup_hs6621.s    %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug_flash.ini...
copy /Y %base%\keil\%ver%\debug_flash.ini     %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621_flash.sct...
copy /Y %base%\keil\%ver%\scatter_hs6621_flash.sct %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6621_flash.s...
copy /Y %base%\keil\%ver%\startup_hs6621_flash.s %base%\keil
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

goto SWITCH_END


:SWITCH_FAIL
echo !!! Switch Fail, Please close all your MDK projects !!!
pause

:SWITCH_END

