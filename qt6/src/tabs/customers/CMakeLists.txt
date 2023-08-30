# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_pluginTarget "TabCustomers")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_HEADERS
  customersconfig.h
  customerseditor.h
  customersdata.h
  customersfinancial.h
  customerssearchbar.h
  customersselectfilter.h
  customersstatusbar.h
  customerstabconfigwidget.h
  customerstablemodel.h
  customerstableview.h
  customerstrustlevel.h
  customerswidget.h
  customersorders.h
  tabcustomers.h
)

SET (plugin_SOURCES
  customerseditor.cpp
  customersdata.cpp
  customersfinancial.cpp
  customerssearchbar.cpp
  customersselectfilter.cpp
  customersstatusbar.cpp
  customerstabconfigwidget.cpp
  customerstablemodel.cpp
  customerstableview.cpp
  customerstrustlevel.cpp
  customerswidget.cpp
  customersorders.cpp
  tabcustomers.cpp
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_pluginTarget} STATIC
 ${plugin_SOURCES}
 ${plugin_HEADERS}
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
