@echo off

echo Removing Particle\src\main.ino
del Particle\src\main.ino

for /f %%F in ('dir /b Common\include') do (
    echo Removing ESP32\include\%%F
    del ESP32\include\%%F

    echo Removing Particle\src\%%F
    del Particle\src\%%F

    mklink /H ESP32\include\%%F Common\include\%%F
    mklink /H Particle\src\%%F Common\include\%%F
)

for /f %%F in ('dir /b Common\src') do (
    echo Removing ESP32\src\%%F
    del ESP32\src\%%F

    echo Removing Particle\src\%%F
    del Particle\src\%%F
    
    mklink /H ESP32\src\%%F Common\src\%%F
    mklink /H Particle\src\%%F Common\src\%%F
)

echo Renaming Particle\src\main.cpp to Particle\src\main.ino 
move Particle\src\main.cpp Particle\src\main.ino

del Particle\src\LoRaTransceiver.cpp

pause
