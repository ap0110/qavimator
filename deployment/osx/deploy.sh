#!/bin/bash
## This will make a standalone application bundle with all needed Qt5 frameworks included

# get the lib path for Qt
qt_lib_dir=$(qtpaths --install-prefix)/lib

# get the architecture of this machine
architecture=$(uname -m)

# clean previous installs
rm -rf ./QAvimator.app
cp -R ./resources/QAvimator.app ./QAvimator.app

# build and install qavimator into the prepared app bundle
mkdir ../../_build
cd ../../_build
cmake .. -DINSTALL_BIN=../deployment/osx/QAvimator.app/Contents/MacOS -DINSTALL_DATA=../deployment/osx/QAvimator.app/Contents/Resources -DCMAKE_OSX_ARCHITECTURES="$architecture" -DCMAKE_BUILD_TYPE=MinSizeRel -DQt5Widgets_DIR="$qt_lib_dir/cmake/Qt5Widgets"
make install

cd ../deployment/osx
# Fix linking and add required framework files to the project. This is optional if system Qt5 frameworks should be used
mkdir -p ./QAvimator.app/Contents/Frameworks

cp -R "$qt_lib_dir/QtCore.framework/Versions/5/QtCore" ./QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtCore ./QAvimator.app/Contents/Frameworks/QtCore
install_name_tool -change "$qt_lib_dir/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore ./QAvimator.app/Contents/MacOS/qavimator

cp -R "$qt_lib_dir/QtGui.framework/Versions/5/QtGui" ./QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtGui ./QAvimator.app/Contents/Frameworks/QtGui
install_name_tool -change "$qt_lib_dir/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore ./QAvimator.app/Contents/Frameworks/QtGui
install_name_tool -change "$qt_lib_dir/QtGui.framework/Versions/5/QtGui" @executable_path/../Frameworks/QtGui ./QAvimator.app/Contents/MacOS/qavimator

cp -R "$qt_lib_dir/QtNetwork.framework/Versions/5/QtNetwork" ./QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtNetwork ./QAvimator.app/Contents/Frameworks/QtNetwork
install_name_tool -change "$qt_lib_dir/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore ./QAvimator.app/Contents/Frameworks/QtNetwork
install_name_tool -change "$qt_lib_dir/QtNetwork.framework/Versions/5/QtNetwork" @executable_path/../Frameworks/QtNetwork ./QAvimator.app/Contents/MacOS/qavimator

cp -R "$qt_lib_dir/QtWidgets.framework/Versions/5/QtWidgets" ./QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtWidgets ./QAvimator.app/Contents/Frameworks/QtWidgets
install_name_tool -change "$qt_lib_dir/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore ./QAvimator.app/Contents/Frameworks/QtWidgets
install_name_tool -change "$qt_lib_dir/QtGui.framework/Versions/5/QtGui" @executable_path/../Frameworks/QtGui ./QAvimator.app/Contents/Frameworks/QtWidgets
install_name_tool -change "$qt_lib_dir/QtWidgets.framework/Versions/5/QtWidgets" @executable_path/../Frameworks/QtWidgets ./QAvimator.app/Contents/MacOS/qavimator

cp -R "$qt_lib_dir/QtOpenGL.framework/Versions/5/QtOpenGL" ./QAvimator.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/QtOpenGL ./QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$qt_lib_dir/QtCore.framework/Versions/5/QtCore" @executable_path/../Frameworks/QtCore ./QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$qt_lib_dir/QtGui.framework/Versions/5/QtGui" @executable_path/../Frameworks/QtGui ./QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$qt_lib_dir/QtWidgets.framework/Versions/5/QtWidgets" @executable_path/../Frameworks/QtWidgets ./QAvimator.app/Contents/Frameworks/QtOpenGL
install_name_tool -change "$qt_lib_dir/QtOpenGL.framework/Versions/5/QtOpenGL" @executable_path/../Frameworks/QtOpenGL ./QAvimator.app/Contents/MacOS/qavimator
