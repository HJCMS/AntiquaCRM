# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "Various")

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${DISCID_INCLUDE_DIRS}
)

SET (plugin_FILES
 various.json
)

ADD_LIBRARY (${_pluginTarget} MODULE
 ${plugin_FILES} variousconfig.h
 tabvarious.h tabvarious.cpp
 variousconfigwidget.h variousconfigwidget.cpp
 variousindex.h variousindex.cpp
 varioussearchbar.h varioussearchbar.cpp
 variousstatusbar.h variousstatusbar.cpp
 variouseditor.h variouseditor.cpp
 varioustableview.h varioustableview.cpp
 varioustablemodel.h varioustablemodel.cpp
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
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
  LIBRARY DESTINATION lib${LIB_SUFFIX}/antiquacrm/tabs
  COMPONENT Plugin
)
