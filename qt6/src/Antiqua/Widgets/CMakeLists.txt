# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_projectLibrary "AntiquaWidgets")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ZLIB_INCLUDE_DIRS}
)

CONFIGURE_FILE (AntiquaWidgets.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaWidgets
)

SET (projectLibrary_FILES
 AntiquaWidgets.cmake
)

SET (projectinc_FILES
 AntiquaWidgets
)

SET (projectLibrary_SOURCES
)

SET (projectLibrary_HEADERS
 ${projectinc_FILES}
)

ADD_LIBRARY (${_projectLibrary} SHARED
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_projectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt6::Gui
  Qt6::Widgets
  AntiquaCRM
)

INSTALL (TARGETS ${_projectLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)

#EOF