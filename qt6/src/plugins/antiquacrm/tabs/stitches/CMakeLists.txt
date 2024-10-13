# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "Stitches")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
 stitches.json
)

SET (plugin_SOURCES
  stitchesconfigwidget.cpp
  stitcheseditor.cpp
  stitchessearchbar.cpp
  stitchesselectfilter.cpp
  stitchesstatusbar.cpp
  stitchestablemodel.cpp
  stitchestableview.cpp
  stitcheswidget.cpp
  tabstitches.cpp
)

SET (plugin_HEADERS
  stitchesconfig.h
  stitchesconfigwidget.h
  stitcheseditor.h
  stitchessearchbar.h
  stitchesselectfilter.h
  stitchesstatusbar.h
  stitchestablemodel.h
  stitchestableview.h
  stitcheswidget.h
  tabstitches.h
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
  DESTINATION lib${LIB_SUFFIX}/antiquacrm/plugins/tabs/tabs
  COMPONENT Plugin
)
