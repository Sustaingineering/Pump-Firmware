@echo off

cd Particle\src

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


cd ..\..\ESP32\include

for /f %%F in ('dir /b ..\..\Common\include') do (
    echo Removing %%F
    del %%F

    mklink  %%F ..\..\Common\include\%%F
)



cd ..\src

for /f %%F in ('dir /b ..\..\Common\src') do (
    echo Removing %%F
    del %%F

    mklink  %%F ..\..\Common\src\%%F
)

cd ..\..\
