# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_sharedLibrary "AntiquaProviders")

INCLUDE_DIRECTORIES (
  ${INTERNAL_LIBRARY_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(CMAKE_ANTIQUA_PROVIDER_INTERFACE "de.hjcms.antiquacrm.provider")
CONFIGURE_FILE (antiquaproviders.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquaproviders.h
)

SET (_library_FILES
 Providers.dox
 antiquaproviders.h.cmake
)

SET (_stl_HEADERS
 AntiquaProviders
 antiquaproviders.h
)

SET (_library_SOURCES
  profiderinterface.cpp
  providersloader.cpp
  provideractiondialog.cpp
  provideractionnavigator.cpp
)

SET (_library_HEADERS
  profiderinterface.h
  providersloader.h
  provideractiondialog.h
  provideractionnavigator.h
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_stl_HEADERS}
 ${_library_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_PROVIDER_INTERFACE="${CMAKE_ANTIQUA_PROVIDER_INTERFACE}")

SET_TARGET_PROPERTIES (${_sharedLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_sharedLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_sharedLibrary} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Network
  Qt6::Sql
  Qt6::Xml
  AntiquaCRM
  AntiquaWidgets
)

IF (CMAKE_HOST_LINUX)
  INSTALL (TARGETS ${_sharedLibrary}
    LIBRARY DESTINATION lib${LIB_SUFFIX}/
    COMPONENT Libraries
    NAMELINK_COMPONENT Development
  )

  INSTALL (FILES ${_library_HEADERS} ${_stl_HEADERS}
    DESTINATION include/AntiquaCRM
    COMPONENT Development
  )
ENDIF()

#EOF
