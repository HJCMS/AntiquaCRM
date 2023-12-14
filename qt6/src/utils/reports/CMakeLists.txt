# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "AntiquaReports")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

CONFIGURE_FILE (antiquareports.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquareports.h
)

ADD_LIBRARY (${_library} STATIC
  antiquareports.h.cmake
  antiquareports.h
  reportsmenu.h reportsmenu.cpp
)

SET_TARGET_PROPERTIES (${_library} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_library}
)

TARGET_LINK_LIBRARIES (${_library} PRIVATE
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
