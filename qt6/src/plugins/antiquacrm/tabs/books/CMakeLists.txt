# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_pluginTarget "Books")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
 books.json
)

SET (plugin_SOURCES
 tabbooks.cpp
 bookswidget.cpp
 bookssearchbar.cpp
 booksselectfilter.cpp
 bookstableview.cpp
 bookstablemodel.cpp
 booksstatusbar.cpp
 bookseditor.cpp
 bookstabconfigwidget.cpp
)

SET (plugin_HEADERS
 booksconfig.h
 tabbooks.h
 bookswidget.h
 bookssearchbar.h
 booksselectfilter.h
 bookstableview.h
 bookstablemodel.h
 booksstatusbar.h
 bookseditor.h
 bookstabconfigwidget.h
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_pluginTarget} MODULE
 ${plugin_SOURCES}
 ${plugin_HEADERS}
 ${plugin_FILES}
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
)

TARGET_LINK_LIBRARIES (${_pluginTarget} PUBLIC
  Qt6::Core
  Qt6::Widgets
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  Qt6::PrintSupport
  AntiquaCRM
  AntiquaWidgets
  AntiquaPrinting
  AntiquaTabs
)

INSTALL (TARGETS ${_pluginTarget}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/tabs
  COMPONENT Plugin
)
