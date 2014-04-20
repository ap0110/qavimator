#!/bin/bash

DEPLOYMENT_DIR=`pwd`

cd ../..

PROJECT_ROOT_DIR=`pwd`
INSTALL_DIR="$PROJECT_ROOT_DIR/_install"
ZIPPABLE_DIR="$PROJECT_ROOT_DIR/QAvimator"

rm -rf _build _install

./configure
if [ $? -ne 0 ]
then
  echo "configure did not run successfully"
  exit $?
fi

make
if [ $? -ne 0 ]
then
  echo "make did not run successfully"
  exit $?
fi

echo "Copying dependencies..."
cp "$QT_LIB_DIR/libQt5Core.so.5" \
   "$QT_LIB_DIR/libQt5Gui.so.5" \
   "$QT_LIB_DIR/libQt5Network.so.5" \
   "$QT_LIB_DIR/libQt5Widgets.so.5" \
   "$QT_LIB_DIR/libQt5OpenGL.so.5" \
   "$QT_LIB_DIR/libicudata.so.51" \
   "$QT_LIB_DIR/libicui18n.so.51" \
   "$QT_LIB_DIR/libicuuc.so.51" \
   "$DEPLOYMENT_DIR/resources/qavimator.sh" \
   "$INSTALL_DIR"

mv "$INSTALL_DIR/qavimator" \
   "$INSTALL_DIR/do_not_run_directly"

# Return to original deployment directory
cd "$DEPLOYMENT_DIR"

FILE_NAME="${APPLICATION_NAME}"
FILE_NAME="${FILE_NAME}_${VERSION_NUMBER}"
FILE_NAME="${FILE_NAME}_${BUILD_NUMBER}"
FILE_NAME="${FILE_NAME}_linux"
FILE_NAME="${FILE_NAME}_${MACHINE_ARCH}"

echo "Archiving and compressing..."

mv "$INSTALL_DIR" "$ZIPPABLE_DIR"
tar -cf "${FILE_NAME}.tar" "$ZIPPABLE_DIR"
gzip -9 "${FILE_NAME}.tar"
mv "$ZIPPABLE_DIR" "$INSTALL_DIR"

echo "Done"

exit 0
