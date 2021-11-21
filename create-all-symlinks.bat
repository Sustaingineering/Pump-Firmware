@echo off

cd Particle\src
call create_symlinks.bat

cd ..\..\ESP32\include
call create_symlinks.bat

cd ..\src
call create_symlinks.bat
