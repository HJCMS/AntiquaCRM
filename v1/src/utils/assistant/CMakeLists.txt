# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaAssistant")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_FILES
 config_p.h.cmake
)

SET (subproject_RESOURCES
 documents.qrc
)

CONFIGURE_FILE (config_p.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/config_p.h
)

SET (subproject_SOURCES
 configpgsql.cpp
 configssl.cpp
 assistant.cpp
 infopage.cpp
 testconfig.cpp
)

SET (subproject_HEADERS
  config_p.h
  configpgsql.h
  configssl.h
  assistant.h
  infopage.h
  testconfig.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  ${subproject_RESOURCES}
  ${subproject_FILES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  AntiquaCRM
)
