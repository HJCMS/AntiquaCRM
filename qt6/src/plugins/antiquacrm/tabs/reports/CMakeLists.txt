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

CONFIGURE_FILE (antiquareports.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquareports.h
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_pluginTarget} MODULE
  ${plugin_FILES}
  antiquareports.h.cmake
  antiquareports.h
  reportsmenu.h reportsmenu.cpp
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
 AntiquaCRM
 AntiquaTabs
 AntiquaWidgets
)

##EOF
