CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)
CMAKE_POLICY (VERSION 2.8.11)

# Enable automoc
SET (CMAKE_AUTOMOC ON)
# Set the compiler standard to C++11 and enable warnings
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")

PROJECT (qavimator)

# If update channel and version number were defined
IF ((DEFINED UPDATE_CHANNEL) AND (DEFINED VERSION_NUMBER))
  # Pass them to the compiler
  ADD_DEFINITIONS (-DUPDATE_CHANNEL=${UPDATE_CHANNEL} -DVERSION_NUMBER="${VERSION_NUMBER}")
ENDIF ((DEFINED UPDATE_CHANNEL) AND (DEFINED VERSION_NUMBER))

# Configurable install variables, use CMAKE_INSTALL_PREFIX to install into target directory
SET (INSTALL_BIN "." CACHE PATH "Executable install path")
SET (INSTALL_DATA "." CACHE PATH "Data install path")

# Initialize and search for Qt5 libraries
SET (QT_USE_QTOPENGL TRUE)
FIND_PACKAGE (Qt5Widgets REQUIRED)

# Search for OpenGL and GLUT
FIND_PACKAGE (OpenGL REQUIRED)
FIND_PACKAGE (GLUT REQUIRED)

INCLUDE_DIRECTORIES (src ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

SUBDIRS (src)

# Install data
INSTALL (DIRECTORY bin/data DESTINATION ${INSTALL_DATA})
