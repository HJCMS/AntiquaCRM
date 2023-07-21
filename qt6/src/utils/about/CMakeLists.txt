# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "AboutDialog")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

QT6_ADD_RESOURCES (_library_RC_FILES licences.qrc)

SET (_library_FILES
 aboutantiquacrm.txt
 aboutpostgresql.txt
 aboutqrencode.txt
 ${_library_RC_FILES}
)

SET (_library_SOURCES
 aboutdialogs.cpp
 infowidget.cpp
 aboutantiquacrm.cpp
 aboutpostgresql.cpp
 aboutqrencode.cpp
)

SET (_library_HEADERS
 aboutdialogs.h
 infowidget.h
 aboutantiquacrm.h
 aboutpostgresql.h
 aboutqrencode.h
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (${_library} STATIC
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_library_FILES}
)

SET_TARGET_PROPERTIES (${_library} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_library}
)

TARGET_LINK_LIBRARIES (${_library} PRIVATE
 Qt6::Core
 Qt6::Widgets
 Qt6::Sql
 Qt6::Network
 Qt6::Xml
 AntiquaCRM
 AntiquaWidgets
)
