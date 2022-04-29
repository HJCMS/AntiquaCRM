# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaCore")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_FILES
  README
)

SET (subproject_SOURCES
  trust.cpp
  trustlist.cpp
)

SET (subproject_HEADERS
  trust.h
  trustlist.h
  AntiquaCRM
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY
  COMPONENT Libraries
  DESTINATION lib${LIB_SUFFIX}/
  PUBLIC_HEADER
  COMPONENT Development
)
