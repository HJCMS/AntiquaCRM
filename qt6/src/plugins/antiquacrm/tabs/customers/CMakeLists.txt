# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_pluginTarget "Customers")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_FILES
  customers.json
)

SET (plugin_HEADERS
  customersconfig.h
  customerseditor.h
  customerssearchbar.h
  customersselectfilter.h
  customersstatusbar.h
  customerstabconfigwidget.h
  customerstablemodel.h
  customerstableview.h
  customerswidget.h
  tabcustomers.h
)

SET (plugin_SOURCES
  customerseditor.cpp
  customerssearchbar.cpp
  customersselectfilter.cpp
  customersstatusbar.cpp
  customerstabconfigwidget.cpp
  customerstablemodel.cpp
  customerstableview.cpp
  customerswidget.cpp
  tabcustomers.cpp
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
  AntiquaTabs
)

INSTALL (TARGETS ${_pluginTarget}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/tabs
  COMPONENT Plugin
)
