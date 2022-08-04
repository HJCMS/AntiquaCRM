# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (booklooker_SOURCES
 booklooker.json
 config.xml
 booklooker.cpp
 booklookerifacewidget.cpp
 booklookerrequester.cpp
 booklookerremoteactions.cpp
)

SET (booklooker_HEADERS
 booklookerconfig.h
 booklooker.h
 booklookerifacewidget.h
 booklookerrequester.h
 booklookerremoteactions.h
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

ADD_LIBRARY (booklooker MODULE
  ${booklooker_SOURCES}
  ${booklooker_HEADERS}
)

SET_TARGET_PROPERTIES (booklooker PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME booklooker
)

TARGET_LINK_LIBRARIES (booklooker PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Xml
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  AntiquaCore
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS booklooker
    LIBRARY DESTINATION lib${LIB_SUFFIX}/antiqua/plugins
    COMPONENT Plugin
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS booklooker
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins/antiqua/
    COMPONENT Plugin
  )
ENDIF (ANTIQUA_LSB_INSTALL)
