if not defined VERSION_NUMBER (
  set VERSION_NUMBER=0.1.0
)
echo VERSION_NUMBER = %VERSION_NUMBER%

if not defined UPDATE_CHANNEL (
  echo.
  
  echo Update channels:
  echo 0 = Development
  echo 1 = Beta
  echo 2 = Release
  set /P UPDATE_CHANNEL=Which channel (0, 1, or 2)? 
  
  echo.
)

if "%UPDATE_CHANNEL%" == "2" (
  set APPLICATION_NAME=QAvimator
  set UPDATE_CHANNEL_NAME=Release
) else if "%UPDATE_CHANNEL%" == "1" (
  set APPLICATION_NAME=QAvimator-Beta
  set UPDATE_CHANNEL_NAME="Beta"
) else (
  set UPDATE_CHANNEL=0
  set APPLICATION_NAME=QAvimator-Development
  set UPDATE_CHANNEL_NAME="Development"
)
echo UPDATE_CHANNEL = %UPDATE_CHANNEL%
echo Update Channel is "%UPDATE_CHANNEL_NAME%"

if not defined GLUT_INCLUDE_DIR (
  set /P GLUT_INCLUDE_DIR=Which directory contains directory "GL", which holds file "glut.h"? 
)
echo GLUT_INCLUDE_DIR = "%GLUT_INCLUDE_DIR%"

if not defined GLUT_LIB_DIR (
  set /P GLUT_LIB_DIR=Which directory contains file "glut.lib"? 
)
echo GLUT_LIB_DIR = "%GLUT_LIB_DIR%"

set PROJECT_ROOT_DIR=%CD%
set BUILD_DIR=%PROJECT_ROOT_DIR%\_build
set INSTALL_DIR=%PROJECT_ROOT_DIR\_install

qtpaths --install-prefix | set QT_LIB_DIR=
set QT_LIB_DIR=%QT_LIB_DIR%\lib
if not %ERRORLEVEL% == 0 (
  echo Could not get path to Qt lib directory
  exit /B %ERRORLEVEL%
)

hg id -n | set BUILD_NUMBER=
if not %ERRORLEVEL% == 0 (
  set BUILD_NUMBER=0
)
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
      -DCMAKE_BUILD_TYPE="Release" ^
      -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
      -DUPDATE_CHANNEL="%UPDATE_CHANNEL%" ^
      -DVERSION_NUMBER="%VERSION_NUMBER%" ^
      -DQt5Widgets_DIR="%QT_LIB_DIR%\cmake\Qt5Widgets" ^
      -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE_DIR%" ^
      -DGLUT_glut_LIBRARY="%GLUT_LIB_DIR%\glut32.lib"

if not %ERRORLEVEL% == 0 (
  echo CMake failed
  exit /B %ERRORLEVEL%
)
      
cmake .. ^
      -G "MinGW Makefiles" ^
      -DCMAKE_BUILD_TYPE="Release" ^
      -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
      -DUPDATE_CHANNEL="%UPDATE_CHANNEL%" ^
      -DVERSION_NUMBER="%VERSION_NUMBER%" ^
      -DQt5Widgets_DIR="%QT_LIB_DIR%\cmake\Qt5Widgets" ^
      -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE_DIR%" ^
      -DGLUT_glut_LIBRARY="%GLUT_LIB_DIR%\glut32.lib"

echo.
echo Done

exit /B 0
