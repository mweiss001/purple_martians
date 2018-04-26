@echo off
cd ..\tests
call ..\batfiles\exe.bat classes
call ..\batfiles\exe.bat dnstest
call ..\batfiles\exe.bat fixaddr
call ..\batfiles\exe.bat gentest
call ..\batfiles\exe.bat getdrvnm
call ..\batfiles\exe.bat rdmtest
cd ..\batfiles
