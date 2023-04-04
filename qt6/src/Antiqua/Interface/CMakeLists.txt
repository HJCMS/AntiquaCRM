# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_sharedLibrary "AntiquaInterface")

INCLUDE_DIRECTORIES (
  ${INTERNAL_INTERFACE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

CONFIGURE_FILE (AntiquaInterface.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaInterface
)

SET (projectLibrary_FILES
 AntiquaInterface.cmake
)

SET (projectinc_FILES
 AntiquaInterface
)

SET (projectLibrary_SOURCES
)

SET (projectLibrary_HEADERS
 ${projectinc_FILES}
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_sharedLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_sharedLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_sharedLibrary} PRIVATE
  Qt6::Core
  AntiquaCRM
  AntiquaWidgets
)

INSTALL (TARGETS ${_sharedLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)

#EOF
