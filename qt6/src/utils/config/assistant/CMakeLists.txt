# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/../
 ${CMAKE_CURRENT_BINARY_DIR}/../
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

QT_ADD_RESOURCES(ASSISTANT_QRC
  ../../../application.qrc
)

ADD_EXECUTABLE(antiqua_assistant
 ${ASSISTANT_QRC}
 main.cpp assistant.h assistant.cpp
 assistantwelcome.h assistantwelcome.cpp
)

SET_TARGET_PROPERTIES(antiqua_assistant PROPERTIES
  OUTPUT_NAME antiqua_assistant
  LINKER_LANGUAGE CXX
  CXX_STANDARD ${ANTIQUACRM_STD_STANDARD}
  WIN32_EXECUTABLE TRUE
)

TARGET_LINK_LIBRARIES(antiqua_assistant PRIVATE
  Qt6::Gui
  Qt6::Network
  Qt6::Widgets
  Qt6::Sql
  Qt6::Xml
  AntiquaCRM
  AntiquaWidgets
  ConfigDialog
)

INSTALL(TARGETS antiqua_assistant
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
  COMPONENT Core
)

##EOF
