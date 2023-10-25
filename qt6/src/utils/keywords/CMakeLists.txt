# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "KeywordsDialog")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (${_library} STATIC
  ${_library_FILES}
  keywordsdialog.h keywordsdialog.cpp
  keywordstable.h keywordstable.cpp
  keywordsedit.h keywordsedit.cpp
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
