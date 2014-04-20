@echo off

set DEPLOYMENT_DIR="%CD%"
set RESOURCE_DIR=%DEPLOYMENT_DIR%\resources

cd ..\..

set PROJECT_ROOT_DIR="%CD%"

rmdir /S /Q _build _install

call configure

if not %ERRORLEVEL% == 0 (
  echo configure did not run successfully
  exit /B %ERRORLEVEL%
)

call make
if not %ERRORLEVEL% == 0 (
  echo make did not run successfully
  exit /B %ERRORLEVEL%
)

qtpaths --binaries-dir > qt_bin.tmp
if not %ERRORLEVEL% == 0 (
  del qt_bin.tmp
  echo Could not get path to the Qt bin directory
  exit /B %ERRORLEVEL%
)
set /P QT_BIN= < qt_bin.tmp
del qt_bin.tmp

cd "%DEPLOYMENT_DIR%"
makensis /V3 /NOCD ^
  /DQT_BIN="%QT_BIN%" ^
  /DPROJECT_ROOT_DIR="%PROJECT_ROOT_DIR%" ^
  /DRESOURCE_DIR="%RESOURCE_DIR%" ^
  /DAPPLICATION_NAME="%APPLICATION_NAME%" ^
  /DAPPLICATION_VERSION="%VERSION_NUMBER%" ^
  /DAPPLICATION_BUILD_NUMBER="%BUILD_NUMBER%" ^
  "%RESOURCE_DIR%\qavimator.nsi"

if not %ERRORLEVEL% == 0 (
  echo NSIS installer make did not run successfully
  exit /B %ERRORLEVEL%
)

exit /B 0
