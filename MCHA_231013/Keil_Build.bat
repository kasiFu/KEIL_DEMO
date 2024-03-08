@echo off
set UV=D:\Software\Keil\UV4\UV4.exe
set UV_PRO_PATH=E:\01.项目\10.无锡逸航\01.Code\客户APP的源码\Update\MCHA_231013\MDK\Mcha.uvprojx
echo Init building …
echo .>build_log.txt
%UV% -j0 -r %UV_PRO_PATH% -o %cd%\build_log.txt
type build_log.txt
echo Done.
pause