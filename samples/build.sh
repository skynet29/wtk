
COMPILO=~/Documents/compilo-win32
BIN=$COMPILO/Bin
export include=$COMPILO/include
export lib=$COMPILO/lib

if [ $# = 1 ]; then
        wine $BIN/cl -c $1.cpp -I../include
        wine $BIN/link /SUBSYSTEM:WINDOWS /OUT:$1.exe $1.obj ../lib/wtk.lib
else
    wine $BIN/cl -c *.cpp -I../include

    for i in `ls *.cpp`; do
        name=$(echo "$i" | cut -f 1 -d '.')
        wine $BIN/link /SUBSYSTEM:WINDOWS /OUT:$name.exe $name.obj ../lib/wtk.lib
    done

fi
