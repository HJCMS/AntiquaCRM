# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
 buchfreund.json
)

SET (plugin_SOURCES
 buchfreund.cpp
)

SET (plugin_HEADERS
 buchfreund.h
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (Buchfreund MODULE
 ${plugin_SOURCES}
 ${plugin_HEADERS}
 ${plugin_FILES}
)

SET_TARGET_PROPERTIES (Buchfreund PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME Buchfreund
)

TARGET_LINK_LIBRARIES (Buchfreund PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  AntiquaCRM
  AntiquaCRMPlugin
)

IF (CMAKE_HOST_UNIX)
  INSTALL (TARGETS Buchfreund
    LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/plugins
    COMPONENT Plugin
  )
ENDIF (CMAKE_HOST_UNIX)
