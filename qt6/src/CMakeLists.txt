# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: MIT
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
IF (CMAKE_HOST_UNIX)
  IF(WITH_ANTIQUACMD)
    ADD_SUBDIRECTORY(antiquacmd)
  ENDIF(WITH_ANTIQUACMD)
ENDIF (CMAKE_HOST_UNIX)

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

## AntiquaCRM Assistant
ADD_SUBDIRECTORY(assistant)

## If not exists - create icon resource file
IF (CMAKE_HOST_UNIX)
  FIND_PROGRAM(QRC_UPDATE_PERL
    "qrciconupdate" "iconupdate.pl"
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}
  )
  IF(EXISTS ${QRC_UPDATE_PERL} AND NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/application.qrc)
    ## FILE(GLOB ICON_FILES ${CMAKE_CURRENT_SOURCE_DIR}/icons/*.png)
    MESSAGE (STATUS "UPDATE application.qrc.")
    EXECUTE_PROCESS (COMMAND ${QRC_UPDATE_PERL}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      ERROR_QUIET
    )
  ENDIF()
ENDIF (CMAKE_HOST_UNIX)

QT_ADD_RESOURCES(APPLICATION_QRC
  application.qrc
)

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
  DataCache
  DeliveryDialog
  DesignationDialog
  KeywordsDialog
  StoragesDialog
  MailTemplatesDialog
  PublisherDialog
  TabSellers
  TabOrders
  TabCustomers
)

## BEGIN:Customized OS Libraries
SET (PROJECT_RESOURCE_WINDOWS "")
IF(CMAKE_HOST_WIN32)
  CONFIGURE_FILE (antiquacrm.rc.cmake ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.rc)
  SET (PROJECT_RESOURCE_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/antiquacrm.rc)
  CONFIGURE_FILE (run.bat.cmake ${CMAKE_CURRENT_BINARY_DIR}/run.bat NEWLINE_STYLE WIN32)
ELSE()
  CONFIGURE_FILE (run.sh.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/run.sh
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
    NEWLINE_STYLE UNIX
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
## END:Customized OS Libraries

ADD_EXECUTABLE(${PROJECTNAME}
 ${APPLICATION_SOURCES}
 ${APPLICATION_QRC}
 ${APPLICATION_FILES}
 ${PROJECT_RESOURCE_WINDOWS}
)

SET_TARGET_PROPERTIES(${PROJECTNAME} PROPERTIES
  OUTPUT_NAME ${PROJECTNAME}
  LINKER_LANGUAGE CXX
  CXX_STANDARD ${ANTIQUACRM_STD_STANDARD}
  WIN32_EXECUTABLE TRUE
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

IF (CMAKE_HOST_LINUX)
  INSTALL(TARGETS ${PROJECTNAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    COMPONENT Core
  )
ENDIF()

#EOF
