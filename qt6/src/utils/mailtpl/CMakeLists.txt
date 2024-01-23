# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "MailTemplatesDialog")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (${_library} STATIC
  mailtemplatesdialog.h mailtemplatesdialog.cpp
  mailtemplatestree.h mailtemplatestree.cpp
  templatestreelist.h templatestreelist.cpp
  macrostreelist.h macrostreelist.cpp
  templateeditor.h templateeditor.cpp
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
 AntiquaWidgets
)

##EOF
