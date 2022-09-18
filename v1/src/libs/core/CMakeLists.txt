# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaCRM")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

CONFIGURE_FILE (aglobal.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/aglobal.h
)

CONFIGURE_FILE (AntiquaCRM.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRM
)

SET (projectLibrary_FILES
 aglobal.h.cmake
 AntiquaCRM.cmake
)

SET (projectLibrary_SOURCES
 asqlsettings.cpp
 asqlcore.cpp
)

SET (projectLibrary_HEADERS
 aglobal.h
 asqlsettings.h
 asqlcore.h
 AntiquaCRM
)

ADD_LIBRARY (${_projectLibrary} SHARED
  ${projectLibrary_HEADERS}
  ${projectLibrary_SOURCES}
  ${projectLibrary_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARAY)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_projectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
)

INSTALL (TARGETS ${_projectLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)
