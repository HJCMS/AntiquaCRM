# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (antiquacrm_LINKER
 AntiquaCore
)

SET (whsoft_SOURCES
  whsoft.cpp
  whsoft.h
)

ADD_LIBRARY (whsoft MODULE
  ${whsoft_SOURCES}
)

SET_TARGET_PROPERTIES (whsoft PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME whsoft
)

TARGET_LINK_LIBRARIES (whsoft PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  ${antiquacrm_LINKER}
)

INSTALL (TARGETS whsoft
  LIBRARY DESTINATION plugins/antiqua/
  COMPONENT Plugin
)
