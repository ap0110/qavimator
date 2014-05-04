#!/bin/bash

DEPLOYMENT_DIR=`pwd`

cd ../..

PROJECT_ROOT_DIR=`pwd`
INSTALL_DIR="$PROJECT_ROOT_DIR/_install"
RENAMED_INSTALL_DIR="$PROJECT_ROOT_DIR/QAvimator"

QT_DIR=`qtpaths --install-prefix`
if [ $? -ne 0 ]
then
  echo "Could not get path to Qt directory"
  exit $?
fi

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

mkdir "$INSTALL_DIR/plugins"
mkdir "$INSTALL_DIR/plugins/platforms"
cp "$QT_DIR/plugins/platforms/libqxcb.so" \
   "$INSTALL_DIR/plugins/platforms"

cp "$QT_DIR/lib/libQt5Core.so.5" \
   "$QT_DIR/lib/libQt5Gui.so.5" \
   "$QT_DIR/lib/libQt5Network.so.5" \
   "$QT_DIR/lib/libQt5Widgets.so.5" \
   "$QT_DIR/lib/libQt5OpenGL.so.5" \
   "$QT_DIR/lib/libQt5DBus.so.5" \
   "$QT_DIR/lib/libicudata.so.51" \
   "$QT_DIR/lib/libicui18n.so.51" \
   "$QT_DIR/lib/libicuuc.so.51" \
   "$DEPLOYMENT_DIR/resources/qavimator.sh" \
   "$DEPLOYMENT_DIR/resources/qt.conf" \
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
