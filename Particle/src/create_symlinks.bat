@echo off

echo Removing main.ino
del main.ino

for /f %%F in ('dir /b ..\..\Common\src') do (
    echo Removing %%F
    del %%F

    mklink  %%F ..\..\Common\src\%%F
)

for /f %%F in ('dir /b ..\..\Common\include') do (
    echo Removing %%F
    del %%F

    mklink  %%F ..\..\Common\include\%%F
)

echo Renaming main.cpp to main.ino 
move main.cpp main.ino

del LoRaTransceiver.cpp