@echo off

for /f %%F in ('dir /b ..\..\Common\include') do (
    echo Removing %%F
    del %%F

    mklink  %%F ..\..\Common\include\%%F
)
