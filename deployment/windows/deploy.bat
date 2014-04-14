@echo off

SET VERSION_NUMBER=0.1.0

echo ""

echo Update channels:
echo 0 = Development
echo 1 = Beta
echo 2 = Release
set /P UPDATE_CHANNEL=Which channel (0, 1, or 2)? 

echo ""

if "%UPDATE_CHANNEL%" == "2" (
  set APPLICATION_NAME=QAvimator
) else if "%UPDATE_CHANNEL%" == "1" (
  set APPLICATION_NAME=QAvimator-Beta
) else (
  set UPDATE_CHANNEL=0
  set APPLICATION_NAME=QAvimator-Development
)

cd ..\..

qtpaths --binaries-dir > qt-bin.temp
set /P QT_BIN= < qt-bin.temp
del qt-bin.temp

hg id -n > build-number.temp
set /P BUILD_NUMBER= < build-number.temp
del build-number.temp
if not defined BUILD_NUMBER (
  set BUILD_NUMBER=0
)

if not exist _build (
  mkdir _build
)

cd _build

set /P GLUT_INCLUDE=-DGLUT_INCLUDE_DIR=
set /P GLUT_LIBRARY=-DGLUT_glut_LIBRARY=

cmake .. -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE="Release" ^
  -DCMAKE_INSTALL_PREFIX=..\_install ^
  -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE%" ^
  -DGLUT_glut_LIBRARY="%GLUT_LIBRARY%" ^
  -DUPDATE_CHANNEL="%UPDATE_CHANNEL%" ^
  -DVERSION_NUMBER="%VERSION_NUMBER%"

cmake .. -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE="Release" ^
  -DCMAKE_INSTALL_PREFIX=..\_install ^
  -DGLUT_INCLUDE_DIR="%GLUT_INCLUDE%" ^
  -DGLUT_glut_LIBRARY="%GLUT_LIBRARY%" ^
  -DUPDATE_CHANNEL="%UPDATE_CHANNEL%" ^
  -DVERSION_NUMBER="%VERSION_NUMBER%"

mingw32-make install

cd ..\deployment\windows
makensis /V3 /NOCD ^
  /DQT_BIN="%QT_BIN%" ^
  /DPROJECT_ROOT_DIR="..\.." ^
  /DRESOURCE_DIR="resources" ^
  /DAPPLICATION_NAME="%APPLICATION_NAME%" ^
  /DAPPLICATION_VERSION="%VERSION_NUMBER%" ^
  /DAPPLICATION_BUILD_NUMBER="%BUILD_NUMBER%" ^
  resources\qavimator.nsi
