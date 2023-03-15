# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaHelper")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_FILES
)

SET (projectLibrary_SOURCES
 helperdialog.cpp
)

SET (projectLibrary_HEADERS
 helperdialog.h
)

ADD_LIBRARY (${_projectLibrary} STATIC
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_projectLibrary}
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
 Qt${QT_VERSION_MAJOR}::Core
 AntiquaCRM
 AntiquaWidgets
)
