@echo off
call env.bat %*

if "%1"=="test" goto :test
if "%2"=="test" goto :test

.build\%VARIANT%\kodama.exe %*
goto :end

:test
if "%1"=="test" set TEST=%2
if "%2"=="test" set TEST=%3
if "%TEST%"=="" (
    for /r %%i in (.build\%VARIANT%\test\*-test.exe) do (
        %%i %*
    )
) else (
    .build\%VARIANT%\test\%TEST%-test.exe %*
)

:end
