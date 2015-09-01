@echo off
call env.bat release %*

python waf %PARAMS%
