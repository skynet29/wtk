
 @echo off


if not [%1] == [] (
    cl -c -GX %1.cpp -I../include
    link /SUBSYSTEM:WINDOWS /OUT:%1.exe %1.obj ../lib/wtk.lib
    goto :end
)

for %%i in (*.cpp) do (
    cl -c %%~ni.cpp -I../include
    link /SUBSYSTEM:WINDOWS /OUT:%%~ni.exe %%~ni.obj ../lib/wtk.lib
)    


:end
    echo Fini
