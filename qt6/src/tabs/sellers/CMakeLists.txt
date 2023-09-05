# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_pluginTarget "TabSellers")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${ANTIQUACRM_PLUGIN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (plugin_SOURCES
  buyerinfo.cpp
  purchaseactionbar.cpp
  purchaseheader.cpp
  purchasetable.cpp
  salesinfo.cpp
  sellersconfigwidget.cpp
  sellerssaleslist.cpp
  sellerssalestab.cpp
  sellerssalewidget.cpp
  sellerswidget.cpp
  tabsellers.cpp
)

SET (plugin_HEADERS
  buyerinfo.h
  purchaseactionbar.h
  purchaseheader.h
  purchasetable.h
  salesinfo.h
  sellersconfig.h
  sellersconfigwidget.h
  sellerssaleslist.h
  sellerssalestab.h
  sellerssalewidget.h
  sellerswidget.h
  tabsellers.h
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
  AntiquaPrinting
  AntiquaProviders
)
