# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaAbout")

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_INCLUDE_DIRS}
)

SET (projectLibrary_LINKER
 AntiquaCRM
)

SET (projectLibrary_FILES
 licences.qrc
 aboutantiquacrm.txt
 aboutpostgresql.txt
 aboutqrencode.txt
)

SET (projectLibrary_SOURCES
 aboutdialogs.cpp
 infowidget.cpp
 aboutantiquacrm.cpp
 aboutpostgresql.cpp
 aboutqrencode.cpp
)

SET (projectLibrary_HEADERS
 aboutdialogs.h
 infowidget.h
 aboutantiquacrm.h
 aboutpostgresql.h
 aboutqrencode.h
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
  Qt${QT_VERSION_MAJOR}::Widgets
  ${projectLibrary_LINKER}
)
