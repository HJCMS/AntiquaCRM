# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "TabOrders")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_HEADERS
  ordersconfig.h
  refundingpages.h
)

ADD_LIBRARY (${_pluginTarget} STATIC
  ${plugin_HEADERS}
  changeordercustomerdialog.h changeordercustomerdialog.cpp
  changecustomerfind.h changecustomerfind.cpp
  orderarticledialog.h orderarticledialog.cpp
  ordersconfigwidget.h ordersconfigwidget.cpp
  orderscostsettings.h orderscostsettings.cpp
  orderscustomerinfo.h orderscustomerinfo.cpp
  orderscustomertoolbar.h orderscustomertoolbar.cpp
  orderseditor.h orderseditor.cpp
  orderssearchbar.h orderssearchbar.cpp
  ordersstatusbar.h ordersstatusbar.cpp
  orderstabledelegate.h orderstabledelegate.cpp
  orderstablemodel.h orderstablemodel.cpp
  orderstableoverview.h orderstableoverview.cpp
  orderstableoverviewmodel.h orderstableoverviewmodel.cpp
  orderstableview.h orderstableview.cpp
  orderstatusactionframe.h orderstatusactionframe.cpp
  orderswidget.h orderswidget.cpp
  refunddialog.h refunddialog.cpp
  refundfinalize.h refundfinalize.cpp
  refundingpage.h refundingpage.cpp
  refundintroduction.h refundintroduction.cpp
  refundselecter.h refundselecter.cpp
  taborders.h taborders.cpp
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
