# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE(CMakeSubTargets.cmake)

ADD_SUBDIRECTORY(data)
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(tabs)
ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(i18n)
ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(widgets)
ADD_SUBDIRECTORY(ordersystem)
ADD_SUBDIRECTORY(plugins)

QT5_ADD_RESOURCES (QRC_FILES resources.qrc)

CONFIGURE_FILE (antiqua.rc.cmake
 ${CMAKE_CURRENT_SOURCE_DIR}/antiqua.rc
)

## AntiquaTabs
CONFIGURE_FILE (AntiquaTabs.cmake
 ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaTabs
)

IF (CMAKE_HOST_WIN32)
SET (PROJECT_RESOURCE_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/antiqua.rc)
ELSE (CMAKE_HOST_WIN32)
SET (PROJECT_RESOURCE_WINDOWS "")
ENDIF (CMAKE_HOST_WIN32)

SET (PROJECT_HEADERS
  antiquaappl.h
  antiquasplashscreen.h
  antiquatimer.h
  antiquawindow.h
  antiquamenubar.h
  antiquaconfigmenus.h
  antiquaviewsmenus.h
  antiquatabwidget.h
  antiquastatusbar.h
  antiquasocketserver.h
  antiquasystemtray.h
  AntiquaTabs
)

SET (PROJECT_SOURCES
  main.cpp
  antiquaappl.cpp
  antiquasplashscreen.cpp
  antiquatimer.cpp
  antiquawindow.cpp
  antiquamenubar.cpp
  antiquaconfigmenus.cpp
  antiquaviewsmenus.cpp
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

