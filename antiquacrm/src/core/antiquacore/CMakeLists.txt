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
  orderstatus.cpp
)

SET (subproject_HEADERS
  trustlist.h
  datafield.h
  orderstatus.h
  AntiquaCRM
  antiqua_global.h
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_subproject}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
)

INSTALL (TARGETS ${_subproject}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${subproject_HEADERS}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}
  COMPONENT Development
)
