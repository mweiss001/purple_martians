@echo off

set used=0


IF [%1]==[headless] (
set used=1

:: create directories
ssh 96.45.13.253 "mkdir -p ~/dev/pm/src  ~/dev/pm/levels ~/dev/pm/data ~/dev/pm/bitmaps ~/dev/pm/o/src"

set cc=pscp -pw zaiden
set dest=m@96.45.13.253:dev/pm

%cc% c:\pm\lin_compile %dest%
%cc% c:\pm\lin_link %dest%
%cc% c:\pm\lin_build_all %dest%
%cc% c:\pm\op %dest%
%cc% c:\pm\pm.cfg %dest%


%cc% c:\pm\bitmaps\sprit001.pm  %dest%/bitmaps/sprit001.pm
%cc% c:\pm\data\level_data.pm %dest%/data/level_data.pm

%cc% -r c:\pm\src m@96.45.13.253:dev/pm
%cc% -r c:\pm\levels\level0* m@96.45.13.253:dev/pm/levels

)

IF [%1]==[test] (
set used=1
ECHO test
)



IF [%1]==[release] (

set used=1

ECHO release


set dp=~/dev/pm


set sc="mkdir -p %dp%/help"

echo %sc%


ssh 96.45.13.253 "%sc%"


)

::ssh 96.45.13.253 "mkdir -p %dp%/help"

::set dp=~/dev/pm

::ssh 96.45.13.253 "mkdir -p %dp%/src  %dp%/levels %dp%/data %dp%/bitmaps %dp%/o/src"

::ssh 96.45.13.253 "mkdir -p %dp%/help"






if %used% EQU 0 (
ECHO: 
ECHO Value Missing - use one from this list
ECHO headless - copies all files needed for headless server
ECHO release - copies all files needed for linux release
ECHO: 

)

