# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaCore")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CURL_INCLUDE_DIR}
)

CONFIGURE_FILE (antiqua_global.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiqua_global.h
)

CONFIGURE_FILE (interface.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/interface.h
)

SET (subproject_FILES
  README
)

SET (subproject_SOURCES
  applsettings.cpp
  trustlist.cpp
  datafield.cpp
  orderstatus.cpp
  orderarticle.cpp
  providerorders.cpp
  pluginloader.cpp
  networker.cpp
  provider.cpp
)

SET (subproject_HEADERS
  applsettings.h
  trustlist.h
  datafield.h
  orderstatus.h
  orderarticle.h
  providerorders.h
  networker.h
  provider.h
  AntiquaCRM
  antiqua_global.h
  pluginloader.h
  interface.h
  AntiquaInterface
  Networking
)

ADD_LIBRARY (${_subproject} SHARED
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_subproject}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  ${CURL_LIBRARIES}
)

INSTALL (TARGETS ${_subproject}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${subproject_HEADERS}
  DESTINATION include/${PROJECT_NAME}
  COMPONENT Development
)
