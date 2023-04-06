# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET(_subWidgetLibrary "KeywordEditor")

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (_subWidgetLibrary_SOURCES
 keywordlistview.cpp
 keywordlabel.cpp
)

SET (_subWidgetLibrary_HEADERS
 keywordlistview.h
 keywordlabel.h
)

ADD_LIBRARY (${_subWidgetLibrary} STATIC
  ${_subWidgetLibrary_HEADERS}
  ${_subWidgetLibrary_SOURCES}
)

SET_TARGET_PROPERTIES (${_subWidgetLibrary} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subWidgetLibrary}
)

TARGET_LINK_LIBRARIES (${_subWidgetLibrary} PRIVATE
 Qt6::Core
 Qt6::Gui
 Qt6::Widgets
 Qt6::Network
 Qt6::Sql
 Qt6::Xml
 AntiquaCRM
)

## EOF
