# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

## Data libraries
ADD_SUBDIRECTORY (data)
ADD_SUBDIRECTORY (icons)
ADD_SUBDIRECTORY (i18n)

## Shared libraries
ADD_SUBDIRECTORY(Antiqua)

SET(ANTIQUACRM_INCLUDE_DIRS
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiqua/Core
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiqua/Widgets
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiqua/Image
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiqua/Printing
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiqua/Providers
  ${CMAKE_CURRENT_SOURCE_DIR}/Antiqua/Tabs
)

## Server
IF(WITH_ANTIQUACMD)
ADD_SUBDIRECTORY(antiquacmd)
ENDIF(WITH_ANTIQUACMD)

## Depends on ANTIQUACRM_INCLUDE_DIRS
ADD_SUBDIRECTORY(plugins)

SET(ANTIQUACRM_PLUGIN_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins
)

## Static libraries
ADD_SUBDIRECTORY(tabs)
ADD_SUBDIRECTORY(utils)

SET(ANTIQUA_APPL_INCLUDE_DIRS
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${ANTIQUACRM_PLUGIN_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

INCLUDE_DIRECTORIES (${ANTIQUA_APPL_INCLUDE_DIRS})

QT_ADD_RESOURCES(APPLICATION_QRC application.qrc)

SET(APPLICATION_FILES
  data/antiquacrm.qcss
)

SET(APPLICATION_SOURCES
 main.cpp
 antiquadialogs.h
 application.h application.cpp
 mainwindow.h mainwindow.cpp
 menubar.h menubar.cpp
 statusbar.h statusbar.cpp
 systemtrayicon.h systemtrayicon.cpp
 switchdatabaseprofile.h switchdatabaseprofile.cpp
 splashscreen.h splashscreen.cpp
)

SET(APPLICATION_LIBRARIES
  AntiquaCRM
  AntiquaWidgets
  AntiquaImage
  AntiquaPrinting
  AntiquaTabs
  AboutDialog
  CompanyDialog
  ConfigDialog
  DeliveryDialog
  DesignationDialog
  KeywordsDialog
  StoragesDialog
  TabSellers
  TabOrders
  TabCustomers
)

IF(Qt6WinExtras_FOUND)
  SET(APPLICATION_LIBRARIES
   ${APPLICATION_LIBRARIES}
   Qt6::WinExtras
  )
ENDIF()

IF(Qt6DBus_FOUND)
  ADD_DEFINITIONS (-DANTIQUACRM_DBUS_ENABLED)
  SET(APPLICATION_LIBRARIES
   ${APPLICATION_LIBRARIES}
   Qt6::DBus
  )
  SET(APPLICATION_SOURCES
   ${APPLICATION_SOURCES}
   abusadaptor.h abusadaptor.cpp
  )
ENDIF()

ADD_EXECUTABLE(${PROJECTNAME}
 ${APPLICATION_SOURCES}
 ${APPLICATION_QRC}
 ${APPLICATION_FILES}
)

TARGET_LINK_LIBRARIES(${PROJECTNAME} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Network
  Qt6::Widgets
  Qt6::Sql
  Qt6::Xml
  ${APPLICATION_LIBRARIES}
)

SET_TARGET_PROPERTIES(${PROJECTNAME} PROPERTIES
  OUTPUT_NAME ${PROJECTNAME}
  LINKER_LANGUAGE CXX
  WIN32_EXECUTABLE TRUE
)

INSTALL(TARGETS ${PROJECTNAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
  COMPONENT Core
)

#EOF
