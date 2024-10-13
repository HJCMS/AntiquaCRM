# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "Views")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
 views.json
)

SET (plugin_SOURCES
  tabviews.cpp
  viewsindex.cpp
  viewsconfigwidget.cpp
  viewstablemodel.cpp
)

SET (plugin_HEADERS
  tabviews.h
  viewsconfig.h
  viewsindex.h
  viewsconfigwidget.h
  viewstablemodel.h
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
  AntiquaCRM
  AntiquaWidgets
  AntiquaTabs
)

INSTALL (TARGETS ${_pluginTarget}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/tabs
  COMPONENT Plugin
)
