@echo off

set DEPLOYMENT_DIR="%CD%"

cd ..\..

set PROJECT_ROOT_DIR="%CD%"

configure
make

qtpaths --binaries-dir | set QT_BIN=
if not %ERRORLEVEL% == 0 (
  echo Could not get path to the Qt bin directory
  exit /B %ERRORLEVEL%
)

cd "%DEPLOYMENT_DIR%"
makensis /V3 /NOCD ^
  /DQT_BIN="%QT_BIN%" ^
  /DPROJECT_ROOT_DIR="%PROJECT_ROOT_DIR%" ^
  /DRESOURCE_DIR="resources" ^
  /DAPPLICATION_NAME="%APPLICATION_NAME%" ^
  /DAPPLICATION_VERSION="%VERSION_NUMBER%" ^
  /DAPPLICATION_BUILD_NUMBER="%BUILD_NUMBER%" ^
  "%DEPLOYMENT_DIR%\resources\qavimator.nsi"

@echo on
