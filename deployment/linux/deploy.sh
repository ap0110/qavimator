#!/bin/bash

VERSION_NUMBER=0.1.0

echo ""

echo Update channels:
echo 0 = Development
echo 1 = Beta
echo 2 = Release
read -p "Which channel (0, 1, or 2)? " UPDATE_CHANNEL

echo ""

if [ "$UPDATE_CHANNEL" == "2" ]
then
  APPLICATION_NAME=QAvimator
elif [ "$UPDATE_CHANNEL" == "1" ]
then
  APPLICATION_NAME=QAvimator-Beta
else
  UPDATE_CHANNEL=0
  APPLICATION_NAME=QAvimator-Development
fi

pushd ../..

MACHINE_ARCH=`uname -m`

QT_LIB_DIR=`qtpaths --install-prefix`
if [ $? -ne 0 ]
then
  echo "Could not get path to Qt lib"
  exit 1
fi
QT_LIB_DIR="${QT_LIB_DIR}/lib"

BUILD_NUMBER=`hg id -n`
# If BUILD_NUMBER is unset, then set it to 0
if [ -z ${BUILD_NUMBER+x} ]
then
  BUILD_NUMBER=0
fi

if [ ! -d _build ]
then
  mkdir _build
fi

cd _build

cmake .. \
  -DCMAKE_BUILD_TYPE="Release" \
  -DCMAKE_INSTALL_PREFIX=../_install \
  -DUPDATE_CHANNEL="$UPDATE_CHANNEL" \
  -DVERSION_NUMBER="$VERSION_NUMBER" \
  -DQt5Widgets_DIR="${QT_LIB_DIR}/cmake/Qt5Widgets"

cmake .. \
  -DCMAKE_BUILD_TYPE="Release" \
  -DCMAKE_INSTALL_PREFIX=../_install \
  -DUPDATE_CHANNEL="$UPDATE_CHANNEL" \
  -DVERSION_NUMBER="$VERSION_NUMBER" \
  -DQt5Widgets_DIR="${QT_LIB_DIR}/cmake/Qt5Widgets"

make install

echo "Copying dependencies..."
cp ${QT_LIB_DIR}/libQt5Core.so.5 \
   ${QT_LIB_DIR}/libQt5Gui.so.5 \
   ${QT_LIB_DIR}/libQt5Network.so.5 \
   ${QT_LIB_DIR}/libQt5Widgets.so.5 \
   ${QT_LIB_DIR}/libQt5OpenGL.so.5 \
   ${QT_LIB_DIR}/libicudata.so.51 \
   ${QT_LIB_DIR}/libicui18n.so.51 \
   ${QT_LIB_DIR}/libicuuc.so.51 \
   ../deployment/linux/resources/qavimator.sh \
   ../_install

mv ../_install/qavimator \
   ../_install/do_not_run_directly

# Return to original deployment directory
popd

FILE_NAME="${APPLICATION_NAME}"
FILE_NAME="${FILE_NAME}_${VERSION_NUMBER}"
FILE_NAME="${FILE_NAME}_${BUILD_NUMBER}"
FILE_NAME="${FILE_NAME}_${MACHINE_ARCH}"

echo "Archiving and compressing..."

mv ../../_install ../../QAvimator
tar -cf "${FILE_NAME}.tar" ../../QAvimator
gzip -9 "${FILE_NAME}.tar"
mv ../../QAvimator ../../_install

echo "Done"
