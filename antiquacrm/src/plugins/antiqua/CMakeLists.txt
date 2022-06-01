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
  whsoft.json
  example-whsoft-order.json
)

ADD_LIBRARY (whsoft MODULE
  ${whsoft_SOURCES}
)

ADD_DEFINITIONS (-DANTIQUACORE_LIBRARAY)

SET_TARGET_PROPERTIES (whsoft PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME whsoft
)

TARGET_LINK_LIBRARIES (whsoft INTERFACE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets
  ${antiquacrm_LINKER}
)

IF (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS whsoft
    LIBRARY DESTINATION lib${LIB_SUFFIX}/antiqua/plugins
    COMPONENT Plugin
  )
ELSE (ANTIQUA_LSB_INSTALL)
  INSTALL (TARGETS whsoft
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins/antiqua/
    COMPONENT Plugin
  )
ENDIF (ANTIQUA_LSB_INSTALL)
