set LIB_PATH=../libs

rem ### relative to absolute
pushd "%LIB_PATH%"
set LIB_PATH=%CD%
popd

set BOOST_PATH=%LIB_PATH%\boost_1_59_0
set BOOST_LIB=%BOOST_PATH%\stage\lib
set GTEST_PATH=%LIB_PATH%\googletest
set QT5_ROOT=%LIB_PATH%\Qt\Qt5.3.2\5.3\msvc2013_64_opengl



rem ### don't edit after this ###

set LIB=%BOOST_LIB%;%QT5_ROOT%\lib
set LIB=%LIB%;%GTEST_PATH%\.build\release\googletest;%GTEST_PATH%\.build\debug\googletest
set LIB=%LIB%;%GTEST_PATH%\.build\release\googlemock;%GTEST_PATH%\.build\debug\googlemock
set INCLUDE=%BOOST_PATH%;%QT5_ROOT%\include;%GTEST_PATH%\googletest\include;%GTEST_PATH%\googlemock\include


rem ### add libs to path to not bother with DLL
setlocal EnableDelayedExpansion
if "%BOOST_BIN%" == "" (
    set BOOST_BIN=%BOOST_LIB%
    set PATH=!PATH!;!BOOST_LIB!
)
if "%QT5_BIN%" == "" (
    set QT5_BIN=!QT5_ROOT!\bin
    set PATH=!PATH!;!QT5_BIN!
)
endlocal & set PATH=%PATH%


rem ### expand params with variant
setlocal EnableDelayedExpansion
set VARIANT=debug
if "%1" == "release" set VARIANT=%1
if "%2" == "" (
    set PARAMS=build_%VARIANT%
) else (
    set PARAMS=
    :params_loop
    if "%2" NEQ "" (
        call :params_variant %2
        shift
        goto :params_loop
    )
)
endlocal & set PARAMS=%PARAMS% & set VARIANT=%VARIANT%
exit /b


:params_variant
for %%i in (clean build install) do (
    if "%1" == "%%i" (
        set PARAMS=!PARAMS! %1_%VARIANT%
        exit /b
    )
)
set PARAMS=!PARAMS! %1
exit /b
