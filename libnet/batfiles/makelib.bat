@echo off
cd ..\lib
call ..\batfiles\obj.bat core\address
call ..\batfiles\obj.bat core\channels
call ..\batfiles\obj.bat core\classes
call ..\batfiles\obj.bat core\config
call ..\batfiles\obj.bat core\connhelp
call ..\batfiles\obj.bat core\conns
call ..\batfiles\obj.bat core\core
call ..\batfiles\obj.bat core\drivers
call ..\batfiles\obj.bat core\fixaddr
call ..\batfiles\obj.bat core\lists
call ..\batfiles\obj.bat core\threads
call ..\batfiles\obj.bat core\timer
call ..\batfiles\obj.bat core\wait
call ..\batfiles\obj.bat drivers\inetaddr
call ..\batfiles\obj.bat drivers\internet
call ..\batfiles\obj.bat drivers\ipx
call ..\batfiles\obj.bat drivers\ipxsocks
call ..\batfiles\obj.bat drivers\local
call ..\batfiles\obj.bat drivers\nonet
call ..\batfiles\obj.bat drivers\serdos
call ..\batfiles\obj.bat drivers\serial
call ..\batfiles\obj.bat drivers\serlinux
call ..\batfiles\obj.bat drivers\template
call ..\batfiles\obj.bat drivers\wsockdos
call ..\batfiles\lib.bat libnet core\* drivers\*
cd ..\batfiles
