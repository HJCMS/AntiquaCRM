# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE(CMakeSubTargets.cmake)

QT5_ADD_RESOURCES (QRC_FILES icons.qrc)

SET (PROJECT_HEADERS
  mapplication.h
  mwindow.h
  searchtoolbar.h
  statusbar.h
  adockwidget.h
  workspace.h
)

SET (PROJECT_SOURCES
  main.cpp
  mapplication.cpp
  mwindow.cpp
  searchtoolbar.cpp
  statusbar.cpp
  adockwidget.cpp
  workspace.cpp
  ${QRC_FILES}
)

ADD_EXECUTABLE(${PROJECTNAME}
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES}
)

TARGET_LINK_LIBRARIES(${PROJECTNAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    ${_link_sub_libs}
)

SET_TARGET_PROPERTIES(${PROJECTNAME} PROPERTIES
  OUTPUT_NAME ${PROJECTNAME}
  LINKER_LANGUAGE CXX
  WIN32_EXECUTABLE TRUE
)

##EOF
