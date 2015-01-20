REM Install panStamp files for Arduino-1.5

@echo off

echo Installing patch...
XCOPY libraries ..\libraries\ /s
XCOPY hardware ..\hardware\ /s
XCOPY app ..\app\ /s
XCOPY win\platform.txt ..\hardware\panstamp\msp430\ /s
XCOPY win\tools ..\hardware\tools\ /s
echo Installation completed

