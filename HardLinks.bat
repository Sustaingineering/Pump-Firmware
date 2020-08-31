rm Particle\src\main.cpp

for /f %%F in ('ls Common\include') do mklink /H ESP32\include\%%F Common\include\%%F
for /f %%F in ('ls Common\include') do mklink /H Particle\src\%%F Common\include\%%F

for /f %%F in ('ls Common\src') do mklink /H ESP32\src\%%F Common\src\%%F
for /f %%F in ('ls Common\src') do mklink /H Particle\src\%%F Common\src\%%F

mv Particle\src\main.cpp Particle\src\main.ino

pause
