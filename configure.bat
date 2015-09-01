@echo off
call env.bat %*

python waf configure %*
