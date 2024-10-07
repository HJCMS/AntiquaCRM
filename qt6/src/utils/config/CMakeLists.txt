# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_library "ConfigDialog")

SET (ANTIQUACRM_INCLUDE_DIRS
 ${ANTIQUACRM_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
)

INCLUDE_DIRECTORIES (
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_LIBRARY (${_library} STATIC
  configdatabase.h configdatabase.cpp
  configdialog.h configdialog.cpp
  configgeneral.h configgeneral.cpp
  configlookandfeel.h configlookandfeel.cpp
  configpageswidget.h configpageswidget.cpp
  configpaths.h configpaths.cpp
  configprinting.h configprinting.cpp
  configprovidersview.h configprovidersview.cpp
  configstoragecache.h configstoragecache.cpp
  configtabsview.h configtabsview.cpp
  configtreewidget.h configtreewidget.cpp
  databaseprofile.h databaseprofile.cpp
  iconthemes.h iconthemes.cpp
  pathsettingsgroup.h pathsettingsgroup.cpp
  paymentrefundsettings.h paymentrefundsettings.cpp
  paymentsettingsgroup.h paymentsettingsgroup.cpp
  pluginlistwidget.h pluginlistwidget.cpp
  pluginlistwidgetitem.h pluginlistwidgetitem.cpp
  printerattachments.h printerattachments.cpp
  printerdevices.h printerdevices.cpp
  printerfonts.h printerfonts.cpp
  printerpaperlayout.h printerpaperlayout.cpp
  printerqrcode.h printerqrcode.cpp
  printersetborder.h printersetborder.cpp
  sslcaselecter.h sslcaselecter.cpp
  sslmode.h sslmode.cpp
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
 AntiquaProviders
 AntiquaWidgets
)

## AntiquaCRM Assistant
ADD_SUBDIRECTORY(assistant)

##EOF
