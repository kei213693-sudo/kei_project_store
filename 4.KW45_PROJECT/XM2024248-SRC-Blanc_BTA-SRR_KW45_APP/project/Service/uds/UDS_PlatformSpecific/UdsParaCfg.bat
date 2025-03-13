REM Please adapt pathes according to your installation
REM Here a Python3 instalation at C:\Users\111\AppData\Local\Programs\Python\Python38
REM SET PYTHON3_DIR=C:\Users\RedLeaf\AppData\Local\Programs\Python\Python38-32
SET UDSPARA_TOOL="UdsParaCfg.exe"
SET CFGFILE="UdsParaCfg.xlsx"

%UDSPARA_TOOL%  -if %CFGFILE%

PAUSE

