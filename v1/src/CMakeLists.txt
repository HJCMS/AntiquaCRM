# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

ADD_SUBDIRECTORY(libs)
#ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(i18n)
#ADD_SUBDIRECTORY(utils)

INCLUDE(CMakeSubTargets.cmake)

QT5_ADD_RESOURCES (QRC_FILES resources.qrc)

CONFIGURE_FILE (antiqua.rc.cmake
 ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECTNAME}.rc
)

IF (CMAKE_HOST_WIN32)
SET (PROJECT_RESOURCE_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECTNAME}.rc)
ELSE (CMAKE_HOST_WIN32)
SET (PROJECT_RESOURCE_WINDOWS "")
ENDIF (CMAKE_HOST_WIN32)

SET (PROJECT_LIBS
 AntiquaCRM
)

SET (PROJECT_HEADERS
  antiquaappl.h
  antiquawindow.h
  antiquamenubar.h
  antiquatabwidget.h
  antiquastatusbar.h
  antiquasocketserver.h
  antiquasystemtray.h
)

SET (PROJECT_SOURCES
  main.cpp
  antiquaappl.cpp
  antiquawindow.cpp
  antiquamenubar.cpp
  antiquatabwidget.cpp
  antiquastatusbar.cpp
  antiquasocketserver.cpp
  antiquasystemtray.cpp
  ${QRC_FILES}
)

ADD_EXECUTABLE(${PROJECTNAME}
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES}
  ${PROJECT_RESOURCE_WINDOWS}
)

TARGET_LINK_LIBRARIES(${PROJECTNAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    ${PROJECT_LIBRARIES}
)

SET_TARGET_PROPERTIES(${PROJECTNAME} PROPERTIES
  OUTPUT_NAME ${PROJECTNAME}
  LINKER_LANGUAGE CXX
  WIN32_EXECUTABLE TRUE
)

INSTALL (TARGETS ${PROJECTNAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
  COMPONENT Core
)

##EOF

