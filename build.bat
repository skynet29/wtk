
 cl -c *.cpp

link /SUBSYSTEM:WINDOWS /OUT:test.exe *.obj user32.lib gdi32.lib comdlg32.lib