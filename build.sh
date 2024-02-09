
COMPILO=~/Documents/compilo-win32
BIN=$COMPILO/Bin
export include=c:\\users\\skynet\\Documents\\compilo-win32\\include
export lib=c:\\users\\skynet\\Documents\\compilo-win32\\lib
mkdir -p build
mkdir -p lib
mkdir -p include

cp *.h include

cd build
wine $BIN/cl -c -DDLLMDZ ../*.cpp

wine $BIN/link /DLL /OUT:../lib/wtk.dll *.obj User32.lib Gdi32.lib Comdlg32.lib
