@echo off

for /f %%F in ('dir /b ..\..\Common\src') do (
    echo Removing %%F
    del %%F

    mklink  %%F ..\..\Common\src\%%F
)