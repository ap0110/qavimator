#!/bin/bash

DEPLOYMENT_DIR=`pwd`

cd ../..

PROJECT_ROOT_DIR=`pwd`
INSTALL_DIR="$PROJECT_ROOT_DIR/_install"
RENAMED_INSTALL_DIR="$PROJECT_ROOT_DIR/QAvimator"

QT_LIB_DIR=`qtpaths --install-prefix`
if [ $? -ne 0 ]
then
  echo "Could not get path to Qt lib directory"
  exit $?
fi
QT_LIB_DIR="$QT_LIB_DIR/lib"

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

read FILE_NAME < .file_name.tmp

echo "Archiving and compressing..."

mv "$INSTALL_DIR" "$RENAMED_INSTALL_DIR"
tar --transform='s/.*\(QAvimator.*\)/\1/' \
    -cf "${FILE_NAME}.tar" \
    "$RENAMED_INSTALL_DIR"
gzip -9 "${FILE_NAME}.tar"
mv "$RENAMED_INSTALL_DIR" "$INSTALL_DIR"

echo "Done"

exit 0
