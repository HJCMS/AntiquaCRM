# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
 abebooks.json
)

SET (plugin_SOURCES
 abebooks.cpp
 abebooksdocument.cpp
)

SET (plugin_HEADERS
 abebooks.h
 abebooksdocument.h
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (AbeBooks MODULE
 ${plugin_SOURCES}
 ${plugin_HEADERS}
 ${plugin_FILES}
)

SET_TARGET_PROPERTIES (AbeBooks PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME AbeBooks
)

TARGET_LINK_LIBRARIES (AbeBooks PUBLIC
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Xml
  AntiquaCRM
)

IF (CMAKE_HOST_UNIX)
  INSTALL (TARGETS AbeBooks
    LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/plugins
    COMPONENT Plugin
  )
ENDIF (CMAKE_HOST_UNIX)
