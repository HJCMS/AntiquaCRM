# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "Networking")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CURL_INCLUDE_DIRS}
)

SET (subproject_LINKER
 AntiquaCore
 Settings
 ${CURL_LIBRARIES}
)

SET (subproject_SOURCES
  networker.cpp
  isbnrequest.cpp
  provider.cpp
)

SET (subproject_HEADERS
  networker.h
  isbnrequest.h
  provider.h
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  ${subproject_LINKER}
)

