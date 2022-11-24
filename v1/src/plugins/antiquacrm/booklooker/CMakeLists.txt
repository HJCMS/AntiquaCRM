# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
 booklooker.json
)

SET (plugin_SOURCES
 booklooker.cpp
 booklookerauthenticator.cpp
)

SET (plugin_HEADERS
 booklooker.h
 booklookerauthenticator.h
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (Booklooker MODULE
 ${plugin_SOURCES}
 ${plugin_HEADERS}
 ${plugin_FILES}
)

SET_TARGET_PROPERTIES (Booklooker PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME Booklooker
)

TARGET_LINK_LIBRARIES (Booklooker PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  AntiquaCRM
)

IF (CMAKE_HOST_UNIX)
  INSTALL (TARGETS Booklooker
    LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/plugins
    COMPONENT Plugin
  )
ENDIF (CMAKE_HOST_UNIX)
