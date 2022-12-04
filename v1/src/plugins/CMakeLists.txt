# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
## Plugin Interface
SET(_projectLibrary "AntiquaCRMPlugin")

SET (_projectLibrary_SOURCE
 aplugininterface.h
 apluginloader.h
 aplugininterface.cpp
 apluginloader.cpp
 AntiquaCRMPlugin
 AntiquaCRMPlugin.cmake
)

CONFIGURE_FILE (AntiquaCRMPlugin.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRMPlugin
)

ADD_LIBRARY (${_projectLibrary} SHARED
 ${_projectLibrary_SOURCE}
)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_projectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${ZLIB_LIBRARIES}
  AntiquaCRM
)

ADD_SUBDIRECTORY(antiquacrm)

