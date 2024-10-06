# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(ANTIQUA_APPL_INCLUDE_DIRS
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${ANTIQUACRM_PLUGIN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

INCLUDE_DIRECTORIES (${ANTIQUA_APPL_INCLUDE_DIRS})

QT_ADD_RESOURCES(ASSISTANT_QRC
  antiqua_assistant.qrc
)

SET(ASSISTANT_FILES
  antiqua_assistant.qrc
)

ADD_EXECUTABLE(antiqua_assistant
 ${ASSISTANT_FILES} ${ASSISTANT_QRC}
 assistant.h assistant.cpp
 configpgsql.h configpgsql.cpp
 configssl.h configssl.cpp
 infopage.h infopage.cpp
 testconfig.h testconfig.cpp
 main.cpp
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
)
