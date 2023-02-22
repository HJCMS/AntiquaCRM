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
ADD_SUBDIRECTORY(plugins)
ADD_SUBDIRECTORY(assistant)

QT5_ADD_RESOURCES (QRC_FILES resources.qrc)

CONFIGURE_FILE (antiquacrm.rc.cmake
 ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.rc
)

## AntiquaTabs
CONFIGURE_FILE (AntiquaTabs.cmake
 ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaTabs
)

IF (CMAKE_HOST_WIN32)
SET (PROJECT_RESOURCE_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.rc)
ELSE (CMAKE_HOST_WIN32)
SET (PROJECT_RESOURCE_WINDOWS "")
ENDIF (CMAKE_HOST_WIN32)

SET (PROJECT_HEADERS
  antiquaappl.h
  antiquasplashscreen.h
  antiquatimer.h
  antiquawindow.h
  antiquadbinfo.h
  antiquamenubar.h
  antiquaconfigmenus.h
  antiquaviewsmenus.h
  antiquatabwidget.h
  antiquadatabasebar.h
  antiquastatusbar.h
  antiquasystemtray.h
  AntiquaTabs
)

SET (PROJECT_SOURCES
  main.cpp
  antiquaappl.cpp
  antiquasplashscreen.cpp
  antiquatimer.cpp
  antiquawindow.cpp
  antiquadbinfo.cpp
  antiquamenubar.cpp
  antiquaconfigmenus.cpp
  antiquaviewsmenus.cpp
  antiquatabwidget.cpp
  antiquadatabasebar.cpp
  antiquastatusbar.cpp
  antiquasystemtray.cpp
  ${QRC_FILES}
)

SET (PROJECT_LINUX_FILES "")
IF (Qt${QT_VERSION_MAJOR}DBus_FOUND)
 GET_TARGET_PROPERTY(QT_DBUSXML2CPP_EXECUTABLE
  Qt${QT_VERSION_MAJOR}::qdbusxml2cpp
  LOCATION
 )
 SET (PROJECT_LIBRARIES
  ${PROJECT_LIBRARIES}
  Qt${QT_VERSION_MAJOR}::DBus
 )
 SET (PROJECT_LINUX_FILES
  abusadaptor.h
  abusadaptor.cpp
 )
 ADD_DEFINITIONS (-DANTIQUACRM_DBUS_ENABLED)
ENDIF ()

ADD_EXECUTABLE(${PROJECTNAME}
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES}
  ${PROJECT_LINUX_FILES}
  ${PROJECT_RESOURCE_WINDOWS}
)

IF (Qt${QT_VERSION_MAJOR}WinExtras_FOUND)
  SET(PROJECT_LIBRARIES
   ${PROJECT_LIBRARIES}
   Qt${QT_VERSION_MAJOR}::WinExtras
  )
ENDIF ()

TARGET_COMPILE_FEATURES(${PROJECTNAME} PUBLIC cxx_std_17)

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

