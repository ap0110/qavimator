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

qtpaths --install-prefix > qt_dir.tmp
if not %ERRORLEVEL% == 0 (
  del qt_dir.tmp
  echo Could not get path to the Qt directory
  exit /B %ERRORLEVEL%
)
set /P QT_DIR= < qt_dir.tmp
del qt_dir.tmp

cd "%DEPLOYMENT_DIR%"
makensis /V3 /NOCD ^
  /DQT_DIR="%QT_DIR%" ^
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
