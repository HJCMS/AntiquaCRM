# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "TabSellers")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${ANTIQUACRM_PLUGIN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/repair
)

ADD_SUBDIRECTORY(repair)

ADD_LIBRARY (${_pluginTarget} STATIC
  sellersconfig.h
  buyerinfo.h buyerinfo.cpp
  popupopenexists.h popupopenexists.cpp
  purchaseactionbar.h purchaseactionbar.cpp
  purchaseheader.h purchaseheader.cpp
  purchasetable.h purchasetable.cpp
  salesinfo.h salesinfo.cpp
  sellersconfigwidget.h sellersconfigwidget.cpp
  sellerssaleslist.h sellerssaleslist.cpp
  sellerssalestab.h sellerssalestab.cpp
  sellerssalewidget.h sellerssalewidget.cpp
  sellerswidget.h sellerswidget.cpp
  tabsellers.h tabsellers.cpp
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
  AntiquaPrinting
  AntiquaProviders
  SellerImportRepair
)
