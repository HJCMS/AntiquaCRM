# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
## Plugin Interface
SET(_projectLibrary "AntiquaCRMPlugin")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ZLIB_INCLUDE_DIRS}
)

SET (_projectLibrary_SOURCE
 aplugininterface.cpp
 apluginloader.cpp
 updatedialog.cpp
 updatedialog.h
 AntiquaCRMPlugin.cmake
)

SET (_projectLibrary_HEADERS
 aplugininterface.h
 apluginloader.h
 updatedialog.h
 AntiquaCRMPlugin
)

CONFIGURE_FILE (AntiquaCRMPlugin.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRMPlugin
)

ADD_LIBRARY (${_projectLibrary} SHARED
 ${_projectLibrary_SOURCE}
 ${_projectLibrary_HEADERS}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_projectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${ZLIB_LIBRARIES}
  AntiquaCRM
)

ADD_SUBDIRECTORY(antiquacrm)

INSTALL (TARGETS ${_projectLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${_projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)

