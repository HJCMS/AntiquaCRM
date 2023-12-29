# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_sharedLibrary "AntiquaTabs")

INCLUDE_DIRECTORIES (
 ${INTERNAL_LIBRARY_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

SET (_library_FILES
 Tabs.dox
 antiquatabs.h.cmake
)

SET(CMAKE_ANTIQUA_TABS_INTERFACE "de.hjcms.antiquacrm.tabs")
CONFIGURE_FILE (antiquatabs.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquatabs.h
)

SET (_stl_HEADERS
 AntiquaTabs
 antiquatabs.h
)

SET (_library_SOURCE
 tabsloader.cpp
 tabsbar.cpp
 tabsinterface.cpp
 tabsindex.cpp
 tabsmenu.cpp
 tabstable.cpp
 tabssearchbar.cpp
 tabsstatusprogress.cpp
 tabsstatusbar.cpp
 tabseditor.cpp
 tabswidget.cpp
 tabseditactionbar.cpp
)

SET (_library_HEADERS
 tabsloader.h
 tabsbar.h
 tabsinterface.h
 tabsindex.h
 tabsmenu.h
 tabstable.h
 tabssearchbar.h
 tabsstatusprogress.h
 tabsstatusbar.h
 tabseditor.h
 tabswidget.h
 tabseditactionbar.h
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${_library_SOURCE}
 ${_library_HEADERS}
 ${_stl_HEADERS}
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

INSTALL (FILES ${library_HEADERS} ${_stl_HEADERS}
 DESTINATION include/AntiquaCRM
 COMPONENT Development
)

##EOF
