@echo off

echo Removing Particle\src\main.ino
rm Particle\src\main.ino

for /f %%F in ('ls Common\include') do (
    echo Removing ESP32\include\%%F
    rm ESP32\include\%%F
    echo Removing Particle\src\%%F
    rm Particle\src\%%F
    mklink /H ESP32\include\%%F Common\include\%%F
    mklink /H Particle\src\%%F Common\include\%%F
)

for /f %%F in ('ls Common\src') do (
    echo Removing ESP32\src\%%F
    rm ESP32\src\%%F
    echo Removing Particle\src\%%F
    rm Particle\src\%%F
    mklink /H ESP32\src\%%F Common\src\%%F
    mklink /H Particle\src\%%F Common\src\%%F
)

echo Renaming Particle\src\main.cpp to Particle\src\main.ino 
mv Particle\src\main.cpp Particle\src\main.ino

rm Particle\src\temp.cpp
rm Particle\src\LoRaTransceiver.cpp

pause
