# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "Booklooker")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY (${_pluginTarget} MODULE
 booklooker.json booklookerconfig.h
 booklooker.h booklooker.cpp
 booklookerconfigwidget.h booklookerconfigwidget.cpp
 booklookeractions.h booklookeractions.cpp
 booklookeroperations.h booklookeroperations.cpp
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
  POSITION_INDEPENDENT_CODE ON
)

TARGET_LINK_LIBRARIES (${_pluginTarget} PUBLIC
  Qt6::Core
  Qt6::Widgets
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  AntiquaCRM
  AntiquaWidgets
  AntiquaProviders
)

INSTALL (TARGETS ${_pluginTarget}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/providers
  COMPONENT Plugin
)

##EOF
