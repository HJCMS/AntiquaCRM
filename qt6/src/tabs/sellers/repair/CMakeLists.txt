# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_sublib "SellerImportRepair")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${ANTIQUACRM_PLUGIN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_sublib} STATIC
  sellerimportrepair.h sellerimportrepair.cpp
  importrepairnavbar.h importrepairnavbar.cpp
  importrepairfinder.h importrepairfinder.cpp
  importrepairedit.h importrepairedit.cpp
)

SET_TARGET_PROPERTIES (${_sublib} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_sublib}
)

TARGET_LINK_LIBRARIES (${_sublib} PUBLIC
  Qt6::Core
  Qt6::Widgets
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  Qt6::PrintSupport
  AntiquaCRM
  AntiquaWidgets
  AntiquaTabs
  AntiquaProviders
)
