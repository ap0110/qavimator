CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)
CMAKE_POLICY (VERSION 2.8.11)
SET (CMAKE_AUTOMOC ON)

PROJECT (qavimator)

# Configurable install variables, use CMAKE_INSTALL_PREFIX to install into target directory
SET (INSTALL_BIN "." CACHE PATH "Executable install path")
SET (INSTALL_DATA "." CACHE PATH "Data install path")

# Initialize and search for Qt5 libraries
SET (QT_USE_QTOPENGL TRUE)
FIND_PACKAGE (Qt5Widgets REQUIRED)

# Search for OpenGL and GLUT
FIND_PACKAGE (OpenGL REQUIRED)
FIND_PACKAGE (GLUT REQUIRED)

INCLUDE_DIRECTORIES (libquat src ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

SUBDIRS (libquat)
SUBDIRS (src)

# Install data
INSTALL (DIRECTORY bin/data DESTINATION ${INSTALL_DATA})
