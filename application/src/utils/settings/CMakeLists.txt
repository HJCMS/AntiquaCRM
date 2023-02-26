# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaSettings")

SET (subproject_LINKER
 AntiquaCRM
 AntiquaWidgets
)

SET (subproject_FILES
  README
  TODO
  SettingsDialog
)

SET (subproject_HEADERS
 configdialog.cpp
 generalsettings.cpp
 pathsettings.cpp
 pgsqlsettings.cpp
 printersetup.cpp
 printerfonts.cpp
 printinglayout.cpp
 printsettings.cpp
 providersettings.cpp
 settingswidget.cpp
)

SET (subproject_SOURCES
 configdialog.h
 generalsettings.h
 pathsettings.h
 pgsqlsettings.h
 printersetup.h
 printerfonts.h
 printinglayout.h
 printsettings.h
 providersettings.h
 settingswidget.h
)

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${INCLUDE_CONFIG_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  ${subproject_FILES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::PrintSupport
  ${subproject_LINKER}
)
