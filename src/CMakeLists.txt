# Sources
SET (QAVI_SRC aboutdialog.cpp
              animation.cpp
              animationmanager.cpp
              animationview.cpp
              avatar.cpp
              avatarmanager.cpp
              bvh.cpp
              bvhnode.cpp
              bvhparser.cpp
              camera.cpp
              filemanager.cpp
              floor.cpp
              joint.cpp
              main.cpp
              newanimation.cpp
              prop.cpp
              propmanager.cpp
              qavimator.cpp
              scene.cpp
              settings.cpp
              settingsdialog.cpp
              slpartsfemale.cpp
              slpartsmale.cpp
              timeline.cpp
              timelineview.cpp
              updatechecker.cpp
              versioning.cpp)

# Interfaces
SET (QAVI_UI  aboutdialog.ui
              mainapplicationform.ui
              settingsdialogform.ui)

# Resources
SET (QAVI_RSC icons.qrc)

# Prepare and generate all needed sources and headers
QT5_ADD_RESOURCES (QAVI_RSC_SRC ${QAVI_RSC})
QT5_WRAP_UI (QAVI_UI_HDR ${QAVI_UI})

# Include path to the generated header files
INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR})

# Compile and link
ADD_EXECUTABLE (qavimator ${QAVI_RSC_SRC} ${QAVI_UI_HDR} ${QAVI_SRC})
QT5_USE_MODULES (qavimator Core OpenGL Gui)
TARGET_LINK_LIBRARIES (qavimator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# Install qavimator
INSTALL (TARGETS qavimator DESTINATION ${INSTALL_BIN})
