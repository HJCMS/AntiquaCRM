# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "AntiquaStatistics")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

CONFIGURE_FILE (antiquastatistics.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquastatistics.h
)

ADD_LIBRARY (${_library} STATIC
  antiquastatistics.h.cmake
  antiquastatistics.h
  statisticsmenu.h statisticsmenu.cpp
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
 Qt6::Charts
 AntiquaCRM
 AntiquaTabs
 AntiquaWidgets
)

##EOF
