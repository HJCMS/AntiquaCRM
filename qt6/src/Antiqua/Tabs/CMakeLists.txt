# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_sharedLibrary "AntiquaTabs")

INCLUDE_DIRECTORIES (
 ${INTERNAL_TABS_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

SET(CMAKE_ANTIQUA_TABS_INTERFACE "de.hjcms.antiquacrm.tabs")
CONFIGURE_FILE (antiquatabs.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquatabs.h
)

SET (_library_FILES
 AntiquaTabs
 antiquatabs.h
 antiquatabs.h.cmake
)

SET (_library_SOURCE
 tabsloader.cpp
 tabsbar.cpp
 tabsinterface.cpp
 tabsindex.cpp
 tabstable.cpp
 tabssearchbar.cpp
 tabsstatusbar.cpp
 tabseditor.cpp
 tabwidget.cpp
 tabseditactionbar.cpp
)

SET (_library_HEADERS
 tabsloader.h
 tabsbar.h
 tabsinterface.h
 tabsindex.h
 tabstable.h
 tabssearchbar.h
 tabsstatusbar.h
 tabseditor.h
 tabwidget.h
 tabseditactionbar.h
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${_library_SOURCE}
 ${_library_HEADERS}
 ${_library_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)
ADD_DEFINITIONS (-DANTIQUACRM_TABS_INTERFACE="${CMAKE_ANTIQUA_TABS_INTERFACE}")

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

INSTALL (TARGETS ${_sharedLibrary}
 LIBRARY DESTINATION lib${LIB_SUFFIX}/
 COMPONENT Libraries
 NAMELINK_COMPONENT Development
)

INSTALL (FILES ${library_HEADERS} AntiquaTabs
 DESTINATION include/AntiquaCRM
 COMPONENT Development
)

##EOF
