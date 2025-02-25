@echo off
rem 脚本传入参数(按顺序)： <BIN_DIR>
setlocal EnableDelayedExpansion
rem echo BIN_DIR:%1

set SYMSTOREPATH=\\DESKTOP-CMR204P\SymStore
set PRODUCT="dissect"
set VERSION="0.0.0"
set COMMENT="comments"
set TIMEOUT_DAYS=60

rem ===========  变量  ===========
set BIN_DIR=%~f1
set SYMSTORE_EXE=symstore
set AGESTORE_EXE=agestore -y

echo 清除%TIMEOUT_DAYS%天前的符号
%AGESTORE_EXE% -days=%TIMEOUT_DAYS% %SYMSTOREPATH%
echo.

%SYMSTORE_EXE% add /r /f "D:\dissect\out\release\halve\halve.pdb" /s %SYMSTOREPATH% /t %PRODUCT% /v %VERSION% /c %COMMENT%
%SYMSTORE_EXE% add /r /f "D:\dissect\out\release\halve\halve.exe" /s %SYMSTOREPATH% /t %PRODUCT% /v %VERSION% /c %COMMENT%

echo.

echo ++ 上传成功！！！！
echo.
