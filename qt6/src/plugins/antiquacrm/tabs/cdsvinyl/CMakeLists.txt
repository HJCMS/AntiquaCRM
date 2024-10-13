# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "CDsVinyl")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/dialog
  ${CMAKE_CURRENT_SOURCE_DIR}/dialog
  ${DISCID_INCLUDE_DIRS}
)

ADD_SUBDIRECTORY(dialog)

SET (plugin_FILES
 cdsvinyl.json
)

SET (plugin_SOURCES
  cdsvinylconfigwidget.cpp
  cdsvinyleditor.cpp
  cdsvinylsearchbar.cpp
  cdsvinylselectfilter.cpp
  cdsvinylstatusbar.cpp
  cdsvinyltablemodel.cpp
  cdsvinyltableview.cpp
  cdsvinylwidget.cpp
  tabcdsvinyl.cpp
)

SET (plugin_HEADERS
  cdsvinylconfig.h
  cdsvinylconfigwidget.h
  cdsvinyleditor.h
  cdsvinylsearchbar.h
  cdsvinylselectfilter.h
  cdsvinylstatusbar.h
  cdsvinyltablemodel.h
  cdsvinyltableview.h
  cdsvinylwidget.h
  tabcdsvinyl.h
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
  CDTracking
)

INSTALL (TARGETS ${_pluginTarget}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/tabs
  COMPONENT Plugin
)
