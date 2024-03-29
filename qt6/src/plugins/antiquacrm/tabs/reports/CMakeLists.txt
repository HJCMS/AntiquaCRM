# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_pluginTarget "Reports")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

SET (plugin_FILES
  reports.json
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_pluginTarget} MODULE
  ${plugin_FILES}
  reportsconfigwidget.h reportsconfigwidget.cpp
  reportstoolbar.h reportstoolbar.cpp
  reportsindex.h reportsindex.cpp
  tabreports.h tabreports.cpp
  reportstableview.h reportstableview.cpp
  reportstablemodel.h reportstablemodel.cpp
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
 AntiquaTabs
 AntiquaWidgets
 AntiquaPrinting
)

##EOF
