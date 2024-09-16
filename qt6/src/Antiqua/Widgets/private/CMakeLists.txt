# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_subWidgetLibrary "AntiquaSubWidgets")

INCLUDE_DIRECTORIES (
 ${INTERNAL_WIDGET_INCLUDE_DIRS}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

REMOVE_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_subWidgetLibrary} STATIC
 lineinputvalidator.h lineinputvalidator.cpp
 keywordlistview.h keywordlistview.cpp
 keywordlabel.h keywordlabel.cpp
 mailtemplatekeys.h mailtemplatekeys.cpp
 mailcommand.h mailcommand.cpp
 mailattachments.h mailattachments.cpp
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_STATIC)

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
