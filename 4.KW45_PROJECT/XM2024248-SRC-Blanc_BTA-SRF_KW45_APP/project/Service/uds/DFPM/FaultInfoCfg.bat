

SET FAULT_CFG_TOOL="FaultInfoCfg.exe"
%SET FAULT_CFG_A2L_TOOL="..\Tools\bsw_tool\GenDfpmA2lFile.exe"%
%SET FAULT_A2L_MODIFY_TOOL="..\Tools\bsw_tool\A2L_modify.exe"%
SET FAULT_GEN_FID_TOOL="GenDsmFid.exe"

%SET BSW_BASE_A2L="..\Bsw\Bsw_base.a2l"%
%SET BSW_A2L="..\Bsw\Bsw.a2l"%

SET CFGFILE="OBDFaultList.xlsx"
SEt OUTPATH=".\\"
SET OUTPUTFILE="dfpm_dfpInfo"


%SET ASW_A2L_PATH="..\App"%
%SET ASW_A2L_FILE_NAME="EMS_project"%
%SET ASW_A2L_SOURCE="..\App\EMS_project.a2l"%

%SET ASW_A2L_OUT_PATH="..\App"%
%SET ASW_A2L_OUT_FILE_NAME="Asw"%

SET ASW_FAULT_LIST_PATH=".\\"
SET ASW_FAULT_LIST_FILE_NAME="OBDFaultList"


SET DSM_FID_OUTPATH=".\\"
SET DSM_FID_OUTPUTFILE="DSM_Fid"

call file_changed.bat %CFGFILE%

if "%errorlevel%"=="0" (
    echo %CFGFILE% not modified	
) else (
	echo %CFGFILE% modified
	echo rebuild OBD cfg files
    %FAULT_CFG_TOOL% -if %CFGFILE% -of %OUTPUTFILE% -ofPath %OUTPATH%	
	echo rebuild OBD cfg a2l files
    %FAULT_CFG_A2L_TOOL% -if %CFGFILE% -of %OUTPUTFILE% -ofPath %OUTPATH%
	echo start merge bsw a2l file
	copy /b %BSW_BASE_A2L%+%OUTPATH%\%OUTPUTFILE%.a2l %BSW_A2L%		
	%FAULT_A2L_MODIFY_TOOL% -ifList %ASW_FAULT_LIST_FILE_NAME% -ifListPath %ASW_FAULT_LIST_PATH% -ifA2L %ASW_A2L_FILE_NAME% -ifA2LPath %ASW_A2L_PATH% -ifOutA2L %ASW_A2L_OUT_FILE_NAME% -ifOutA2LPath %ASW_A2L_OUT_PATH%
	echo gen fid
	%FAULT_GEN_FID_TOOL% -if %CFGFILE% -of %DSM_FID_OUTPUTFILE% -ofPath %DSM_FID_OUTPATH%		
	goto endproc
)



:endproc

pause
