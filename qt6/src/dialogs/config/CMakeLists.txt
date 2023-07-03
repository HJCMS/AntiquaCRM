# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "ConfigDialog")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (_library_HEADERS
 configdialog.cpp
 configtreewidget.cpp
 configgeneral.cpp
 configpaths.cpp
 configprinting.cpp
 configlookandfeel.cpp
 configprovidersview.cpp
 configtabsview.cpp
 paymentsettingsgroup.cpp
 pathsettingsgroup.cpp
 pluginlistwidget.cpp
 printerfonts.cpp
 printerattachments.cpp
 printersetborder.cpp
 printerpaperlayout.cpp
 printerqrcode.cpp
 printerdevices.cpp
)

SET (_library_SOURCES
 configdialog.h
 configtreewidget.h
 configgeneral.h
 configpaths.h
 configprinting.h
 configlookandfeel.h
 configprovidersview.h
 configtabsview.h
 paymentsettingsgroup.h
 pathsettingsgroup.h
 pluginlistwidget.h
 printerfonts.h
 printerattachments.h
 printersetborder.h
 printerpaperlayout.h
 printerqrcode.h
 printerdevices.h
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (${_library} STATIC
  ${_library_HEADERS}
  ${_library_SOURCES}
  ${_library_FILES}
)

SET_TARGET_PROPERTIES (${_library} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_library}
)

TARGET_LINK_LIBRARIES (${_library} PRIVATE
 Qt6::Core
 Qt6::Widgets
 Qt6::Network
 Qt6::Sql
 Qt6::PrintSupport
 Qt6::Xml
 AntiquaCRM
 AntiquaTabs
 AntiquaWidgets
)
