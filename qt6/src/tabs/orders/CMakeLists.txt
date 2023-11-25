# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_pluginTarget "TabOrders")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_SOURCES
  refunddialog.cpp
  refundingpage.cpp
  refundintroduction.cpp
  refundselecter.cpp
  refundfinalize.cpp
  orderarticledialog.cpp
  ordersconfigwidget.cpp
  orderscostsettings.cpp
  orderscustomerinfo.cpp
  orderseditor.cpp
  orderssearchbar.cpp
  ordersstatusbar.cpp
  orderstatusactionframe.cpp
  orderstabledelegate.cpp
  orderstablemodel.cpp
  orderstableoverview.cpp
  orderstableoverviewmodel.cpp
  orderstableview.cpp
  orderswidget.cpp
  taborders.cpp
)

SET (plugin_HEADERS
  refunddialog.h
  refundingpage.h
  refundingpages.h
  refundintroduction.h
  refundselecter.h
  refundfinalize.h
  ordersconfig.h
  orderarticledialog.h
  ordersconfigwidget.h
  orderscostsettings.h
  orderscustomerinfo.h
  orderseditor.h
  orderssearchbar.h
  ordersstatusbar.h
  orderstatusactionframe.h
  orderstabledelegate.h
  orderstablemodel.h
  orderstableoverview.h
  orderstableoverviewmodel.h
  orderstableview.h
  orderswidget.h
  taborders.h
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
  AntiquaPrinting
  AntiquaTabs
)
