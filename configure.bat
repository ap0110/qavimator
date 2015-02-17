@echo off

set PROJECT_ROOT_DIR=%CD%

if not defined VERSION_NUMBER (
  set VERSION_NUMBER=0.1.1.450
)
echo VERSION_NUMBER = %VERSION_NUMBER%

if not defined UPDATE_CHANNEL (
  echo.
  
  echo Update channels:
  echo 0 = Development
  echo 1 = Beta
  echo 2 = Release
  set /P UPDATE_CHANNEL="Which channel (0, 1, or 2)? "
  
  echo.
)

if "%UPDATE_CHANNEL%" == "2" (
  set APPLICATION_NAME=QAvimator
  set UPDATE_CHANNEL_NAME=Release
) else if "%UPDATE_CHANNEL%" == "1" (
  set APPLICATION_NAME=QAvimator-Beta
  set UPDATE_CHANNEL_NAME=Beta
) else (
  set UPDATE_CHANNEL=0
  set APPLICATION_NAME=QAvimator-Development
  set UPDATE_CHANNEL_NAME=Development
)
echo UPDATE_CHANNEL = %UPDATE_CHANNEL%
echo Update Channel is "%UPDATE_CHANNEL_NAME%"

if not defined BUILD_TYPE (
  echo.
  
  echo Build types:
  echo 0 = Debug
  echo 1 = Release
  set /P BUILD_TYPE="Which build type (0 or 1)? "
  
  echo.
)

if "%BUILD_TYPE%" == "1" (
  set BUILD_TYPE_NAME=Release
) else (
  set BUILD_TYPE=0
  set BUILD_TYPE_NAME=Debug
)
echo BUILD_TYPE = %BUILD_TYPE%
echo Build type is "%BUILD_TYPE_NAME%"

set PROJECT_ROOT_DIR=%CD%
set BUILD_DIR=%PROJECT_ROOT_DIR%\_build
set INSTALL_DIR=%PROJECT_ROOT_DIR%\_install

qtpaths --install-prefix > qt_lib_dir.tmp
if not %ERRORLEVEL% == 0 (
  del qt_lib_dir.tmp
  echo Could not get path to Qt lib directory
  exit /B %ERRORLEVEL%
)
set /P QT_LIB_DIR= < qt_lib_dir.tmp
set QT_LIB_DIR=%QT_LIB_DIR%\lib
del qt_lib_dir.tmp

hg id -n > build_number.tmp
if not %ERRORLEVEL% == 0 (
  set BUILD_NUMBER=0
) else (
  set /P BUILD_NUMBER= < build_number.tmp
)
del build_number.tmp
echo BUILD_NUMBER = "%BUILD_NUMBER%"

if not exist "%BUILD_DIR%" (
  mkdir "%BUILD_DIR%"
)

if not exist "%INSTALL_DIR%" (
  mkdir "%INSTALL_DIR%"
)

cd "%BUILD_DIR%"

cmake .. ^
      -G "MinGW Makefiles" ^
      -DCMAKE_BUILD_TYPE="%BUILD_TYPE_NAME%" ^
      -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
      -DUPDATE_CHANNEL="%UPDATE_CHANNEL%" ^
      -DVERSION_NUMBER="%VERSION_NUMBER%" ^
      -DQt5Widgets_DIR="%QT_LIB_DIR%\cmake\Qt5Widgets"

if not %ERRORLEVEL% == 0 (
  echo CMake did not run successfully
  exit /B %ERRORLEVEL%
)
      
cmake .. ^
      -G "MinGW Makefiles" ^
      -DCMAKE_BUILD_TYPE="%BUILD_TYPE_NAME%" ^
      -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
      -DUPDATE_CHANNEL="%UPDATE_CHANNEL%" ^
      -DVERSION_NUMBER="%VERSION_NUMBER%" ^
      -DQt5Widgets_DIR="%QT_LIB_DIR%\cmake\Qt5Widgets"
      
cd "%PROJECT_ROOT_DIR%"

echo.
echo Done

exit /B 0
