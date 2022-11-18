# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_projectLibrary "AntiquaCRM")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CURL_INCLUDE_DIRS}
)

CONFIGURE_FILE (aglobal.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/aglobal.h
)

CONFIGURE_FILE (AntiquaCRM.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRM
)

CONFIGURE_FILE (AntiquaCRMPlugin.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/AntiquaCRMPlugin
)

SET (projectLibrary_FILES
 aglobal.h.cmake
 AntiquaCRM.cmake
 AntiquaCRMPlugin.cmake
)

SET (projectinc_FILES
 AntiquaCRM
 AntiquaCRMPlugin
 AGlobal
 ASettings
)

SET (projectLibrary_SOURCES
 acompleterdata.cpp
 acurl.cpp
 aeuropeancountries.cpp
 anetworker.cpp
 anetworkiface.cpp
 anetworkrequest.cpp
 aplugininterface.cpp
 apluginloader.cpp
 aproviderorders.cpp
 asettings.cpp
 ashareddatafiles.cpp
 asqlcore.cpp
 asqldataquery.cpp
 asqlfiles.cpp
 asqlprofile.cpp
 asqlquerymodel.cpp
 asqlsettings.cpp
 atxsocket.cpp
 ataxcalculator.cpp
)

SET (projectLibrary_HEADERS
 acompleterdata.h
 acurl.h
 aeuropeancountries.h
 aglobal.h
 anetworker.h
 anetworkiface.h
 anetworkrequest.h
 aplugininterface.h
 apluginloader.h
 aproviderorders.h
 asettings.h
 ashareddatafiles.h
 asqlcore.h
 asqldataquery.h
 asqlfiles.h
 asqlprofile.h
 asqlquerymodel.h
 asqlsettings.h
 atxsocket.h
 ataxcalculator.h
 ${projectinc_FILES}
)

ADD_LIBRARY (${_projectLibrary} SHARED
 ${projectLibrary_SOURCES}
 ${projectLibrary_HEADERS}
 ${projectLibrary_FILES}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

SET_TARGET_PROPERTIES (${_projectLibrary} PROPERTIES
  VERSION ${ANTIQUACRM_VERSION_STRING}
  SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
  OUTPUT_NAME ${_projectLibrary}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_projectLibrary} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${CURL_LIBRARIES}
)

INSTALL (TARGETS ${_projectLibrary}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/
  COMPONENT Libraries
  NAMELINK_COMPONENT Development
)

INSTALL (FILES ${projectLibrary_HEADERS}
  DESTINATION include/AntiquaCRM
  COMPONENT Development
)
