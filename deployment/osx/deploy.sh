#!/bin/bash
## This will make a standalone application bundle with all needed Qt5 frameworks included

DEPLOYMENT_DIR=`pwd`

cd ../..

PROJECT_ROOT_DIR=`pwd`
INSTALL_DIR="$PROJECT_ROOT_DIR/_install"

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

cd "$INSTALL_DIR"

cp -R "$DEPLOYMENT_DIR/resources/.background" "$INSTALL_DIR"
ln -s /Applications "$INSTALL_DIR/Applications"

# Fix linking and add required framework files to the project. This is optional if system Qt5 frameworks should be used
mkdir -p QAvimator.app/Contents/Frameworks

cp -R "$QT_LIB_DIR/QtCore.framework/Versions/5/QtCore" QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtCore QAvimator.app/Contents/Frameworks/QtCore
install_name_tool -change "$QT_LIB_DIR/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore QAvimator.app/Contents/MacOS/qavimator

cp -R "$QT_LIB_DIR/QtGui.framework/Versions/5/QtGui" QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtGui QAvimator.app/Contents/Frameworks/QtGui
install_name_tool -change "$QT_LIB_DIR/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore QAvimator.app/Contents/Frameworks/QtGui
install_name_tool -change "$QT_LIB_DIR/QtGui.framework/Versions/5/QtGui" @executable_path/../Frameworks/QtGui QAvimator.app/Contents/MacOS/qavimator

cp -R "$QT_LIB_DIR/QtNetwork.framework/Versions/5/QtNetwork" QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtNetwork QAvimator.app/Contents/Frameworks/QtNetwork
install_name_tool -change "$QT_LIB_DIR/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore QAvimator.app/Contents/Frameworks/QtNetwork
install_name_tool -change "$QT_LIB_DIR/QtNetwork.framework/Versions/5/QtNetwork" @executable_path/../Frameworks/QtNetwork QAvimator.app/Contents/MacOS/qavimator

cp -R "$QT_LIB_DIR/QtWidgets.framework/Versions/5/QtWidgets" QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtWidgets QAvimator.app/Contents/Frameworks/QtWidgets
install_name_tool -change "$QT_LIB_DIR/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore QAvimator.app/Contents/Frameworks/QtWidgets
install_name_tool -change "$QT_LIB_DIR/QtGui.framework/Versions/5/QtGui" @executable_path/../Frameworks/QtGui QAvimator.app/Contents/Frameworks/QtWidgets
install_name_tool -change "$QT_LIB_DIR/QtWidgets.framework/Versions/5/QtWidgets" @executable_path/../Frameworks/QtWidgets QAvimator.app/Contents/MacOS/qavimator

cp -R "$QT_LIB_DIR/QtOpenGL.framework/Versions/5/QtOpenGL" QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtOpenGL QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$QT_LIB_DIR/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$QT_LIB_DIR/QtGui.framework/Versions/5/QtGui" @executable_path/../Frameworks/QtGui QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$QT_LIB_DIR/QtWidgets.framework/Versions/5/QtWidgets" @executable_path/../Frameworks/QtWidgets QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$QT_LIB_DIR/QtOpenGL.framework/Versions/5/QtOpenGL" @executable_path/../Frameworks/QtOpenGL QAvimator.app/Contents/MacOS/qavimator

# change back to the original directory (deployment/osx)
cd "$DEPLOYMENT_DIR"
# clean previous deployments
rm -f *.dmg

SIZE=`du -sh "$INSTALL_DIR" | sed 's/\([0-9]*\)M.*/\1/'`
SIZE=`echo $SIZE | awk '{print int($1 + 1.5)}'`
if [ $? -ne 0 ]
then
  echo "Could not get size of _install folder"
  exit $?
fi

echo "Creating disk image..."
hdiutil create -srcfolder "$INSTALL_DIR" -volname QAvimator -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${SIZE}M temp.dmg

echo "Attaching disk image..."
DEVICE=$(hdiutil attach -readwrite -noverify temp.dmg | egrep '^/dev/' | sed 1q | awk '{print $1}')

pushd /Volumes/QAvimator

echo "Setting installation window appearance..."
echo '
  tell application "Finder"
    tell disk "QAvimator"
      open
      set current view of container window to icon view
      set toolbar visible of container window to false
      set statusbar visible of container window to false
      set the bounds of container window to {100, 100, 711, 466}
      set viewOptions to the icon view options of container window
      set arrangement of viewOptions to not arranged
      set icon size of viewOptions to 112
      set background picture of viewOptions to file ".background:'background.png'"
      set position of item "QAvimator.app" of container window to {160, 180}
      set position of item "Applications" of container window to {460, 180}
      close
      open
      update without registering applications
    end tell
  end tell
' | osascript

popd

echo "Detaching disk image..."
hdiutil detach $DEVICE

read FILE_NAME < .file_name.tmp

echo "Compressing disk image..."
hdiutil convert -format UDZO -imagekey zlib-level=9 -o "${FILE_NAME}.dmg" temp.dmg

# clean temporary files
echo "Cleaning temporary files..."
rm -rf temp.dmg

echo "Done"

exit 0
