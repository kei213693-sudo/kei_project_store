@echo off

SET INPUT_FILE_PATH=%1
SET OUTPUT_FILE_PATH=%2

SET start_address=0x00020000
SET end_address=0x000FF81F
SET HEX_VIEW=.\HexView\hexview.exe
SET REMAP_RANGE=%start_address%-%end_address%

%HEX_VIEW% /s %INPUT_FILE_PATH% /FP:0xFF /FR:%REMAP_RANGE% /XS:32:1 -o %OUTPUT_FILE_PATH%

ECHO HexView command executed successfullys