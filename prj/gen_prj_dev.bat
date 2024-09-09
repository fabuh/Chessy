@echo off

set PROJECT_DIR=%~dp0premake
set MAKETOOL=%~dp0premake/tool/premake5

pushd %PROJECT_DIR%
%MAKETOOL% vs2022
popd