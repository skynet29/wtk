
mkdir build
mkdir lib
mkdir include

copy src\*.h include

cd build
del *.obj

cl -c -DDLLMDZ ../src/*.cpp

link /DLL /OUT:../lib/wtk.dll *.obj User32.lib Gdi32.lib Comdlg32.lib Comctl32.lib

cd ..
copy lib\wtk.dll samples