#!/bin/bash

PROJECT_ROOT_DIR=../..
PROJECT_ROOT_DIR_ESCAPED=\\.\\.\\/\\.\\.
INSTALL_DIR="$PROJECT_ROOT_DIR/_install"
INSTALL_DIR_ESCAPED="$PROJECT_ROOT_DIR_ESCAPED\\/_install"

pushd "$PROJECT_ROOT_DIR"

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

# Return to original deployment directory
popd

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
   "$QT_DIR/lib/libicudata.so.52" \
   "$QT_DIR/lib/libicui18n.so.52" \
   "$QT_DIR/lib/libicuuc.so.52" \
   "./resources/qavimator.sh" \
   "./resources/qt.conf" \
   "./resources/README" \
   "$INSTALL_DIR"

read FILE_NAME < .file_name.tmp
read APPLICATION_NAME < .app_name.tmp

rm -rf "$APPLICATION_NAME"
mkdir "$APPLICATION_NAME"
ls -1 "$INSTALL_DIR" \
  | sed "s/\\(.*\\)/$INSTALL_DIR_ESCAPED\\/\\1/" \
  | xargs cp -at "$APPLICATION_NAME"

echo "Archiving and compressing..."

rm -rf "${FILE_NAME}.tar" "${FILE_NAME}.tar.gz"
tar --transform='s/.*\(QAvimator.*\)/\1/' \
    -cf "${FILE_NAME}.tar" \
    "$APPLICATION_NAME"
gzip -9 "${FILE_NAME}.tar"

rm -rf "$APPLICATION_NAME"

echo "Cleaning temporary files..."

rm -rf .file_name.tmp .app_name.tmp

echo "Done"

exit 0
