# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
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
 booksindex.cpp
 bookssearchbar.cpp
 booksselectfilter.cpp
 bookstableview.cpp
 bookstablemodel.cpp
 booksstatusbar.cpp
 bookseditor.cpp
 bookstabconfigwidget.cpp
 catalogsearchbutton.cpp
)

SET (plugin_HEADERS
 booksconfig.h
 tabbooks.h
 booksindex.h
 bookssearchbar.h
 booksselectfilter.h
 bookstableview.h
 bookstablemodel.h
 booksstatusbar.h
 bookseditor.h
 bookstabconfigwidget.h
 catalogsearchbutton.h
)

ADD_LIBRARY (${_pluginTarget} MODULE
 ${plugin_SOURCES}
 ${plugin_HEADERS}
 ${plugin_FILES}
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
  POSITION_INDEPENDENT_CODE ON
)

TARGET_LINK_LIBRARIES (${_pluginTarget} PRIVATE
  Qt6::Core
  Qt6::Widgets
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  Qt6::PrintSupport
  AntiquaCRM
  AntiquaWidgets
  AntiquaImage
  AntiquaPrinting
  AntiquaTabs
)

INSTALL (TARGETS ${_pluginTarget}
  DESTINATION lib${LIB_SUFFIX}/antiquacrm/plugins/tabs/books
  COMPONENT Plugin
)
