@echo off

setlocal EnableDelayedExpansion

set used=0


set srv=96.45.13.253

set srv=scat




set srv_dir=dev/purple_martians
set srv_usr=m

set scp_dest=%srv_usr%@%srv%:%srv_dir%


IF [%1]==[lin_release] (
set used=1
echo.
echo Make Linux Release
echo.

CALL :SUB_lin_make_and_get_release 

)


IF [%1]==[lin_build] (
set used=1
echo.
echo Linux Build
echo.

CALL :SUB_lin_build_all 


)


IF [%1]==[lin_build_and_release] (
set used=1
echo.
echo Linux Build
echo.

CALL :SUB_lin_build_all 

CALL :SUB_lin_make_and_get_release 


)



IF [%1]==[git_reclone] (
CALL :SUB_erase_and_reclone_from_git 
)


IF [%1]==[copy_all] (
CALL :SUB_copy_all 
)


IF [%1]==[copy_src] (
CALL :SUB_copy_src 
)

IF [%1]==[copy_levels] (
CALL :SUB_copy_levels 
)

IF [%1]==[copy_data] (
CALL :SUB_copy_data 
)

IF [%1]==[copy_cfg] (
CALL :SUB_copy_cfg 
)





if %used% EQU 0 (
ECHO: 
ECHO Value Missing - use one from this list
ECHO lin_release
ECHO lin_build
ECHO lin_build_and_release
ECHO git_reclone
ECHO copy_all
ECHO copy_src
ECHO copy_levels
ECHO copy_data
ECHO copy_cfg
ECHO: 

)


GOTO :EOF


:SUB_get_targz 
set cmd1=pscp -pw zaiden %scp_dest%/*.tar.gz C:\pm
echo %cmd1%
%cmd1%
EXIT /B

:SUB_lin_build_all 
set cmd2=ssh %srv% "cd ~/dev/purple_martians; ./lin_build_all;"
echo %cmd2%
%cmd2%
EXIT /B

:SUB_lin_make_and_get_release 
set cmd3=ssh %srv% "cd ~/dev/purple_martians; ./lin_make_release;"
echo %cmd3%
%cmd3%

set cmd1=pscp -pw zaiden %scp_dest%/*.tar.gz C:\pm
echo %cmd1%
%cmd1%



EXIT /B

:SUB_lin_build_and_make 
set cmd4=ssh %srv% "cd ~/dev/purple_martians; ./lin_build_all ./lin_make_release;"
echo %cmd4%
%cmd4%
EXIT /B


:SUB_erase_and_reclone_from_git 

set used=1

echo.
echo Erase and Reclone from Git
echo.

set cmd5=ssh %srv% "cd ~/dev; rm purple_martians -rf; git clone --depth=1 https://github.com/mweiss001/purple_martians; cd purple_martians; chmod 777 -R .; chown m:m -R .;"
echo %cmd5%
%cmd5%
EXIT /B

:SUB_copy_all 
set used=1
echo.
echo Copy all with scp recursive
echo.

REM copy all with scp recursive
REM also copies the large hidden .git dir
REM dir must exist before copy 
set cmd6=pscp -pw zaiden -r c:\pm\* %scp_dest%

echo %cmd6%
%cmd6%
EXIT /B

:SUB_copy_src 
set used=1
echo.
echo Copy src with scp
echo.

REM dir must exist before copy 
set cmd6=pscp -pw zaiden c:\pm\src\* %scp_dest%/src

echo %cmd6%
%cmd6%
EXIT /B



:SUB_copy_levels
set used=1
echo.
echo Copy levels with scp
echo.

REM dir must exist before copy 
set cmd7=pscp -pw zaiden c:\pm\levels\* %scp_dest%/levels

echo %cmd7%
%cmd7%
EXIT /B


:SUB_copy_data
set used=1
echo.
echo Copy data with scp
echo.

REM dir must exist before copy 
set cmd8=pscp -pw zaiden c:\pm\data\* %scp_dest%/data

echo %cmd8%
%cmd8%
EXIT /B


:SUB_copy_cfg
set used=1
echo.
echo Copy pm.cfg with scp
echo.

set cmd9=pscp -pw zaiden c:\pm\pm.cfg %scp_dest%

echo %cmd9%
%cmd9%
EXIT /B







:EOF

