@echo off

setlocal EnableDelayedExpansion

set used=0


set srv=96.45.13.253
set srv_dir=dev/purple_martians
set srv_usr=m

set scp_dest=%srv_usr%@%srv%:%srv_dir%

REM echo.
REM echo scp_dest: %scp_dest%

REM echo.
REM echo arg1:[%1]



IF [%1]==[lin_release] (
set used=1
echo.
echo Make Linux Release
echo.

REM CALL :SUB_lin_build_and_make 
CALL :SUB_lin_make_release 
CALL :SUB_copy_targz_to_win

)



IF [%1]==[git_reclone] (
set used=1
echo.
echo Erase and Reclone from Git
echo.

CALL :SUB_erase_and_reclone_from_git 

)












IF [%1]==[copy_all] (
set used=1
echo.
echo Copy All
echo.


REM copy all with scp recursive
REM also copies the large hidden .git dir
REM dir must exist before copy 
REM set ca_cmd=pscp -pw zaiden -r c:\pm\* %scp_dest%
REM echo %ca_cmd%S
REM %ca_cmd%

REM create with shallow git clone
REM much smaller
REM dir must not exist before clone
set ca_cmd=ssh %srv% "rm %srv_dir% -rf; cd dev; git clone --depth=1 https://github.com/mweiss001/purple_martians; cd purple_martians; chmod 777 ./op; ./op;"

echo %ca_cmd%

@echo on

%ca_cmd%

@echo off

)





IF [%1]==[create_dir] (
set used=1

echo.
echo Create directories
echo.

set cd_cmd=ssh "%srv% mkdir -p ~/%srv_dir%/bitmaps ~/%srv_dir%/data ~/%srv_dir%/docs ~/%srv_dir%/help ~/%srv_dir%/levels ~/%srv_dir%/libnet ~/%srv_dir%/src ~/%srv_dir%/src ~/%srv_dir%/levels ~/%srv_dir%/data ~/%srv_dir%/bitmaps ~/%srv_dir%/o/src"

echo %cd_cmd%
echo.

REM %cd_cmd%

)


IF [%1]==[release] (

set used=1

ECHO release

set dp=~/dev/pm

set sc="mkdir -p %dp%/help"

echo %sc%

REM ssh 96.45.13.253 "%sc%"


)


REM  ssh 96.45.13.253 "mkdir -p %dp%/help"
REM set dp=~/dev/pm
REM ssh 96.45.13.253 "mkdir -p %dp%/src  %dp%/levels %dp%/data %dp%/bitmaps %dp%/o/src"
REM ssh 96.45.13.253 "mkdir -p %dp%/help"


if %used% EQU 0 (
ECHO: 
ECHO Value Missing - use one from this list
ECHO headless - copies all files needed for headless server
ECHO release - copies all files needed for linux release
ECHO: 

)



GOTO :EOF


:SUB_copy_targz_to_win 
set cmd1=pscp -pw zaiden %scp_dest%/*.tar.gz C:\pm
echo %cmd1%
%cmd1%
EXIT /B

:SUB_lin_build_all 
set cmd2=ssh %srv% "cd ~/dev/purple_martians; ./lin_build_all;"
echo %cmd2%
%cmd2%
EXIT /B

:SUB_lin_make_release 
set cmd3=ssh %srv% "cd ~/dev/purple_martians; ./lin_make_release;"
echo %cmd3%
%cmd3%
EXIT /B

:SUB_lin_build_and_make 
set cmd4=ssh %srv% "cd ~/dev/purple_martians; ./lin_build_all ./lin_make_release;"
echo %cmd4%
%cmd4%
EXIT /B


:SUB_erase_and_reclone_from_git 
set cmd5=ssh %srv% "cd ~/dev; rm purple_martians -rf; git clone --depth=1 https://github.com/mweiss001/purple_martians; cd purple_martians; chmod 777 -R .; chown m:m -R .;"
echo %cmd5%
%cmd5%
EXIT /B






:EOF

