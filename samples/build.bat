
 cl -c *.cpp -I../include

link /SUBSYSTEM:WINDOWS /OUT:test1.exe test1.obj ../lib/wtk.lib
link /SUBSYSTEM:WINDOWS /OUT:test2.exe test2.obj ../lib/wtk.lib