
@echo off

set base="%~dp0"
set ver=p1

echo Copy armcc.via...
copy /Y "%base%\keil\%ver%\armcc.via"           "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armclang.via...
copy /Y "%base%\keil\%ver%\armclang.via"        "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armcc_allroles.via...
copy /Y "%base%\keil\%ver%\armcc_allroles.via"  "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armclang_allroles.via...
copy /Y "%base%\keil\%ver%\armclang_allroles.via"  "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink.via...
copy /Y "%base%\keil\%ver%\armlink.via"         "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink6.via...
copy /Y "%base%\keil\%ver%\armlink6.via"         "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink_allroles.via...
copy /Y "%base%\keil\%ver%\armlink_allroles.via" "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy armlink6_allroles.via...
copy /Y "%base%\keil\%ver%\armlink6_allroles.via" "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug.ini...
copy /Y "%base%\keil\%ver%\debug.ini"           "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621.sct...
copy /Y "%base%\keil\%ver%\scatter_hs6621.sct"  "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621_armclang.sct...
copy /Y "%base%\keil\%ver%\scatter_hs6621_armclang.sct"  "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6621.s...
copy /Y "%base%\keil\%ver%\startup_hs6621.s"    "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy debug_flash.ini...
copy /Y "%base%\keil\%ver%\debug_flash.ini"     "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621_flash.sct...
copy /Y "%base%\keil\%ver%\scatter_hs6621_flash.sct" "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy scatter_hs6621_flash_armclang.sct...
copy /Y "%base%\keil\%ver%\scatter_hs6621_flash_armclang.sct" "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy startup_hs6621_flash.s...
copy /Y "%base%\keil\%ver%\startup_hs6621_flash.s" "%base%\keil"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy GCC fiels
echo Copy autoconf_version.h
copy /Y "%base%\gcc\%ver%\autoconf_version.h" "%base%\gcc"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy onmicro.gdb
copy /Y "%base%\gcc\%ver%\onmicro.gdb" "%base%\gcc"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy ldscripts.mk
copy /Y "%base%\gcc\%ver%\ldscripts.mk" "%base%\gcc"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy libs.mk
copy /Y "%base%\gcc\%ver%\libs.mk" "%base%\gcc"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

echo Copy libs_allroles.mk
copy /Y "%base%\gcc\%ver%\libs_allroles.mk" "%base%\gcc"
if %ERRORLEVEL% NEQ 0 goto SWITCH_FAIL

goto SWITCH_END


:SWITCH_FAIL
echo !!! Switch Fail, Please close all your MDK projects !!!
pause

:SWITCH_END

