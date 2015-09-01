@echo off
call env.bat debug %*

echo %PARAMS%
python waf %PARAMS%
