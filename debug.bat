@echo off
call env.bat debug %*

python waf %PARAMS%
