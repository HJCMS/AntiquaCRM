# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_staticLibrary "AntiquaTabs")

INCLUDE_DIRECTORIES (ANTIQUACRM_TABS_INCLUDE_DIRS
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (${ANTIQUACRM_TABS_INCLUDE_DIRS})

SET (_staticLibrary_SOURCE
 tabsbar.cpp
 tabsindex.cpp
 tabstable.cpp
 tabssearchbar.cpp
 tabsstatusbar.cpp
 tabseditor.cpp
)

SET (_staticLibrary_HEADERS
 tabsbar.h
 tabsindex.h
 tabstable.h
 tabssearchbar.h
 tabsstatusbar.h
 tabseditor.h
)

CONFIGURE_FILE (AntiquaTabs.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaTabs
)

ADD_LIBRARY (${_staticLibrary} STATIC
 ${_staticLibrary_SOURCE}
 ${_staticLibrary_HEADERS}
 AntiquaTabs.cmake
)

SET_TARGET_PROPERTIES (${_staticLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_staticLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_staticLibrary} PRIVATE
 Qt6::Core
 Qt6::Gui
 Qt6::Widgets
 Qt6::Network
 Qt6::Sql
 Qt6::Xml
 AntiquaCRM
 AntiquaWidgets
)

#ADD_SUBDIRECTORY(books)
#ADD_SUBDIRECTORY(customers)
#ADD_SUBDIRECTORY(cdsvinyl)
#ADD_SUBDIRECTORY(orders)
#ADD_SUBDIRECTORY(providers)
#ADD_SUBDIRECTORY(prints)
#ADD_SUBDIRECTORY(views)
#ADD_SUBDIRECTORY(reports)
#ADD_SUBDIRECTORY(various)
