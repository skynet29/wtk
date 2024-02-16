
cl -c *.cpp

link /SUBSYSTEM:CONSOLE /OUT:debug.exe *.obj kernel32.lib

