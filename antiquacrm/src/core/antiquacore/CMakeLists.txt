# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaCore")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

CONFIGURE_FILE (antiqua_global.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiqua_global.h
)

SET (subproject_FILES
  README
)

SET (subproject_SOURCES
  trustlist.cpp
  datafield.cpp
)

SET (subproject_HEADERS
  trustlist.h
  datafield.h
  AntiquaCRM
  antiqua_global.h
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

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
