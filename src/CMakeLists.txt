# Sources
SET (QAVI_SRC	animation.cpp
			animationview.cpp
			bvh.cpp
			bvhnode.cpp
			camera.cpp
			iktree.cpp
			keyframelist.cpp
			main.cpp
			math3d.cpp
			prop.cpp
			qavimator.cpp
			rotation.cpp
			settings.cpp
			settingsdialog.cpp
			slpartsfemale.cpp
			slpartsmale.cpp
			timeline.cpp
			timelineview.cpp)

# Headers
SET (QAVI_MOC_HDR 	animation.h
			animationview.h
			bvh.h
			bvhnode.h
			camera.h
			iktree.h
			keyframelist.h
			math3d.h
			playstate.h
			prop.h
			qavimator.h
			rotation.h
			settings.h
			settingsdialog.h
			slparts.h
			timeline.h
			timelineview.h)
			
# Interfaces
SET (QAVI_UI 	mainapplicationform.ui
			settingsdialogform.ui)
			
# Resources
SET (QAVI_RSC	icons.qrc)

# Prepare and generate all needed sources and headers
QT5_ADD_RESOURCES (QAVI_RSC_SRC ${QAVI_RSC})
QT5_WRAP_UI (QAVI_UI_HDR ${QAVI_UI})
QT5_WRAP_CPP (QAVI_MOC_SRC ${QAVI_MOC_HDR})

# Include path to the generated header files
INCLUDE_DIRECTORIES (${CMAKE_BINARY_DIR}/src)

# Set defines
ADD_DEFINITIONS (-DQAVIMATOR_DATAPATH="${INSTALL_DATA_PATH}")

# Compile and link
ADD_EXECUTABLE (qavimator ${QAVI_SRC} ${QAVI_MOC_SRC} ${QAVI_RSC_SRC} ${QAVI_UI_HDR})
QT5_USE_MODULES (qavimator Core OpenGL Gui)
TARGET_LINK_LIBRARIES (qavimator ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} quat)

# Install qavimator
INSTALL (TARGETS qavimator DESTINATION ${INSTALL_BIN})
