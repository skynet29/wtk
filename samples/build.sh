
COMPILO=~/Documents/compilo-win32
BIN=$COMPILO/Bin
export include=$COMPILO/include
export lib=$COMPILO/lib

wine $BIN/cl -c *.cpp -I../include

wine $BIN/link /SUBSYSTEM:WINDOWS /OUT:test1.exe test1.obj ../lib/wtk.lib
wine $BIN/link /SUBSYSTEM:WINDOWS /OUT:test2.exe test2.obj ../lib/wtk.lib
