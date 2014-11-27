@echo off

set DEPLOYMENT_DIR=%CD%
set RESOURCE_DIR=%DEPLOYMENT_DIR%\resources

cd ..\..

set PROJECT_ROOT_DIR=%CD%

if exist _build (
  rmdir /S /Q _build
)
if exist _install (
  rmdir /S /Q _install
)

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

set ZIP_DIR_NAME=%APPLICATION_NAME%

if exist "%ZIP_DIR_NAME%" (
  rmdir /S /Q "%ZIP_DIR_NAME%"
)
mkdir "%ZIP_DIR_NAME%"
xcopy /E /Y /B "%PROJECT_ROOT_DIR%\_install\*" "%ZIP_DIR_NAME%"

copy "%QT_DIR%\bin\icudt52.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\icuin52.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\icuuc52.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\libgcc_s_dw2-1.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\libstdc++-6.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\libwinpthread-1.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\Qt5Core.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\Qt5Gui.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\Qt5Network.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\Qt5OpenGL.dll" "%ZIP_DIR_NAME%"
copy "%QT_DIR%\bin\Qt5Widgets.dll" "%ZIP_DIR_NAME%"

set ZIP_FILE_NAME=%APPLICATION_NAME%_%VERSION_NUMBER%.%BUILD_NUMBER%_Windows.zip
7z a -r "%ZIP_FILE_NAME%" "%ZIP_DIR_NAME%"

rmdir /S /Q "%ZIP_DIR_NAME%"

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
